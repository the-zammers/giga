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
#include "config.h" // readConfig
#include "copypaste.h"
#include <stdlib.h>
#include <string.h>

struct editor_status E;
struct tab_status T;


int main(int argc, char *argv[]){

  E.config_path = "giga.conf";
  
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
    else if(ch==KEY_IC){
      E.mode = !E.mode;
      if(!E.mode) helpbar_alert("editor mode set to INSERT");
      else helpbar_alert("editor mode set to REPLACE");
    }
    else if(ch==KEY_CTRL('r')){
      readConfig(E.config_path);
      struct tab_status *tab = E.tabs[0];
      free_doc(tab->data);
      strncpy(tab->path, E.help_path, LINE_SIZE);
      tab->data=readFile(tab->path, NULL);
      tab->curr_line = tab->data;
      tab->first_line = tab->curr_line;
      init_cursor(tab);
      helpbar_alert("giga reloaded!");
    }

    readwrite_keyhandler(ch);
    tab_keyhandler(ch);
    cursor_keyhandler(ch);
    modify_keyhandler(ch);

    if(ch==KEY_CTRL('f')){
      char *match;
      char needle[LINE_SIZE];
      helpbar_input("String to search for: ", needle, "");
      for(struct line *node = T.first_line; node; node = node->next){
        match = strstr(node->str, needle); // case sensitive
        if(match){
          T.line_goal = node->line_num;
          T.cx = match - (node->str);
          break;
        }
      }
    }

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
