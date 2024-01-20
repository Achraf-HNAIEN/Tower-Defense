#include <MLV/MLV_all.h>
#include <time.h>
#include <getopt.h>

#include "game.h"
#include "graph.h"
#include "grid.h"
#include "monstre.h"
#include "tower.h"
#include "message.h"

// https://elearning.univ-eiffel.fr/pluginfile.php/477685/mod_resource/content/5/projet-tower-defense-compressed.pdf

int main(int argc, char*argv[]) {
  int opt;

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0} // Marqueur de fin des options
    };

    while ((opt = getopt_long(argc, argv, "h", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                print_help();
                return 0;
            default:
                return 0; 
        }
    }

  srand(time(NULL));
  MLV_create_window("Tower Defense Grid", "TD Grid", WIDTH * CELL_SIZE + 200,
                    HEIGHT * CELL_SIZE);

  // Initialize game structure
  Game game = {.wave = 1,
               .mana = 150,
               .mana_max = 2000,
               .path = NULL,
               .pathSize = 0,
               .quit = 0,
               .nb_tower = 0,
               .level_mana = 0,
               .level_gemme_in_shop = 0,
               .want_to_place_tower = 0,
               .tower_count = 0,
               .wavesHead = NULL,// Head of the waves linked list
               .inventory_size = 0,
               .has_start = 0,
               .gemme_selected = -1,  // -1 mean no gemme selected
               .numVisualProjectiles = 0,
               .fusion_slot1 = NULL,
               .fusion_slot2 = NULL};
  for (int i = 0; i < 6; i++) {
    game.inventaire[i] = NULL;
  }

  memset(game.visualProjectiles, 0, sizeof(game.visualProjectiles));
     
  Wave* currentWave;      // Current wave pointer for iteration

  const int frameDelay = 1000 / 60;
  MLV_change_frame_rate(60);
  generatePath(game.grid, &game.path, &game.pathSize);

  int previousTime = MLV_get_time();
  int last_wave_time = previousTime;
  drawAll(&game, NULL,0);

  // LOOP
  while (!game.quit) {
    int currentTime = MLV_get_time();
    float deltaTime = (currentTime - previousTime) / 1000.0f;
    previousTime = currentTime;
    MLV_actualise_window();

    // Update and handle each wave
    currentWave = game.wavesHead;
    while (currentWave != NULL) {
      moveMonsters(currentWave->monsters, game.path, game.pathSize, deltaTime,&game);
      currentWave = currentWave->next;
    }
    
    // Handle events
    MLV_Event event;
    int mouse_x, mouse_y;
    MLV_Button_state state;
    MLV_Keyboard_button key;
    MLV_Mouse_button mouse_button;

    //get event
    event = MLV_get_event(&key, NULL, NULL, NULL, NULL, &mouse_x, &mouse_y,
                          &mouse_button, &state);
    MLV_flush_event_queue();


    if (event == MLV_MOUSE_BUTTON && state == MLV_RELEASED && mouse_button == MLV_BUTTON_LEFT) {
      handle_left_click(mouse_x, mouse_y, &game);
    }
    else if ( event == MLV_MOUSE_BUTTON && state == MLV_RELEASED && mouse_button == MLV_BUTTON_RIGHT && mouse_x <= WIDTH*CELL_SIZE ){
        try_remove_gemme_on_tower(&game, mouse_x, mouse_y);
    }

    // Check for new wave trigger
    else if (game.has_start && ((event == MLV_KEY && key == MLV_KEYBOARD_SPACE &&
               state == MLV_RELEASED) || currentTime - last_wave_time >= WAVE_INTERVAL * 1000)) {

      handle_new_wave(&game, currentTime, &last_wave_time);

    } else if (!game.has_start && (event == MLV_KEY && key == MLV_KEYBOARD_SPACE 
                  && state == MLV_RELEASED)) {
      game.has_start = !game.has_start;
      game.wavesHead = initializeWave(game.wave, game.path, game.pathSize);
      last_wave_time = MLV_get_time();
    } else if (event == MLV_KEY && key == MLV_KEYBOARD_ESCAPE && state == MLV_RELEASED) {
      game.quit++;
    }
    
    if (game.fusion_slot1 && game.fusion_slot2) {
      try_fusion(&game);
    }

    game.next_wave_time = (WAVE_INTERVAL - (currentTime - last_wave_time) / 1000);

    // Draw everything
    update_game(&game, deltaTime, mouse_x, mouse_y);
  
    // Frame rate control
    int frameTime = MLV_get_time() - currentTime;
    if (frameTime < frameDelay) {
        MLV_delay_according_to_frame_rate(frameDelay, frameTime);   
    }
    
  }
  MLV_draw_adapted_text_box((WIDTH * CELL_SIZE ) / 2 - 100,
                            HEIGHT * CELL_SIZE / 2 - 50, "GAME OVER", 20,
                            MLV_COLOR_RED, MLV_COLOR_RED, MLV_COLOR_WHITE,
                            MLV_TEXT_CENTER);
  MLV_actualise_window();
  MLV_wait_seconds(3);
  // Clean up resources
free_game_resources(&game); 
  return 0;
}