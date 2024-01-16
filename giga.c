#include <ncurses.h>
#include "giga.h"
#include "setup.h" // setup, resize
#include "cursor.h" // cursor_keyhandler
#include "util.h" // err
#include "read.h" // readwrite_keyhandler
#include "modify.h" // modify_keyhandler
#include "helpbar.h" // helpbar_alert
#include "visual.h" // refresh_all
#include "tabs.h" // tab_keyhandler
#include "copypaste.h"
#include <stdlib.h>
#include <string.h>

struct editor_status E;
struct tab_status T;


int main(int argc, char *argv[]){

  E.config_path = "giga.conf";
  E.help_path = "help.txt";
  
  setup(argc>1 ? argv[1] : "");

  int ch;
  int * marked = malloc(sizeof(int) * 2);
  char buffer[LINE_SIZE];
  while(1){
    ch = getch();

    helpbar_default();

    if(ch==KEY_CTRL('q')){
      free(marked);
      break;
    }
    else if(ch==KEY_RESIZE){
      resize();
    }
    else if(ch==KEY_IC || ch==KEY_CTRL('e')){
      E.mode = !E.mode;
      if(!E.mode) helpbar_alert("editor mode set to INSERT");
      else helpbar_alert("editor mode set to REPLACE");
    }
    
    readwrite_keyhandler(ch);
    tab_keyhandler(ch);
    cursor_keyhandler(ch);
    modify_keyhandler(ch);


    if(ch==KEY_CTRL('z') && !E.isMarked){
      marked = marking(T.cx_real, T.cy);
      E.isMarked = 1;
    }
    else if(ch==KEY_CTRL('c') && E.isMarked){
      copy_text(marked, T.cx_real, T.cy, buffer);
      E.isMarked = 0;
    }
    else if((ch==KEY_CTRL('n') || ch==KEY_CTRL('v')) && buffer != NULL){
      paste_text(buffer);
      refresh_all();
    }
    
    refresh_all();
  }
  return 0;
}
