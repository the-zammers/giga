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
  char buffer[LINE_SIZE];
  while(1){
    ch = getch();

    helpbar_default();

    if(ch==KEY_CTRL('q')){
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

    if(T.isMarked && T.line_goal!=T.marked[1]){
      T.isMarked = 0;
      helpbar_alert("Mark removed!");
    }

    if(ch==KEY_CTRL('z') && !T.isMarked){
      marking(T.cx_real, T.line_goal);
      T.isMarked = 1;
      helpbar_alert("Mark placed!");
    }
    else if(ch==KEY_CTRL('z') && T.isMarked){
      T.isMarked = 0;
      helpbar_alert("Mark removed!");
    }
    else if(ch==KEY_CTRL('c') && T.isMarked){
      copy_text(T.marked, T.cx_real, T.line_goal, buffer);
      T.isMarked = 0;
      helpbar_alert("Copied!");
    }
    else if((ch==KEY_CTRL('n') || ch==KEY_CTRL('v')) && buffer){
      paste_text(buffer);
      helpbar_alert("Pasted!");
    }
    
    refresh_all();
  }
  return 0;
}
