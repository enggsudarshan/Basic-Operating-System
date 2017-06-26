// filesys_data.h, to be included in main.c as part of kernel data
// this means extern.h must also follow what's happening here

#ifndef _FILESYS_DATA_H
#define _FILESYS_DATA_H_

//***************************** FileSys Data *****************************

char sleep_bin[]= {
          #include "build/sleep.bin" // this must be in a separate line
       };

char hello_bin[]= {
          #include "build/hello.bin" // this must be in a separate line
       };

#define SLEEP_SIZE (sizeof(sleep_bin))
#define HELLO_SIZE (sizeof(hello_bin))

char help_txt_data[] = {
   'I', '\'', 'm', ' ', 's', 'o', 'r', 'r', 'y', ',', ' ', 'b', 'u', 't',
   ' ', 'm', 'y', ' ', 'c', 'a', 'p', 'a', 'c', 'i', 't', 'y', ' ', 't',
   'o', ' ', 'a', 'n', 's', 'w', 'e', 'r', 'i', 'n', 'g', ' ', 'y', 'o',
   'u', 'r', ' ', 'q', 'u', 'e', 's', 't', 'i', 'o', 'n', 's', ' ', 'i',
   's', ' ', 'l', 'i', 'm', 'i', 't', 'e', 'd', '.', '\n', 'Y', 'o', 'u',
   ' ', 'm', 'u', 's', 't', ' ', 'a', 's', 'k', ' ', 't', 'h', 'e', ' ',
   'r', 'i', 'g', 'h', 't', ' ', 'q', 'u', 'e', 's', 't', 'i', 'o', 'n',
   '.', '\n', '\0'
};
#define HELP_TXT_SIZE ( sizeof( help_txt_data ) )

char note_txt_data[] = {
   'A', ' ', 'f', 'a', 'i', 'l', 'u', 'r', 'e', ' ', 'i', 's', ' ', 'o',
   'f', 't', 'e', 'n', ' ', 'p', 'r', 'e', 'c', 'u', 'r', 's', 'o', 'r',
   'e', 'd', ' ', 'b', 'y', ' ', '1', '0', '0', '0', ' ', 'e', 'x', 'c',
   'u', 's', 'e', 's', '.', '\n', '\0'
};
#define NOTE_TXT_SIZE ( sizeof( note_txt_data ) )

// C compiler joins lines below and adds '\0' at the end (not for char arrays above).
char index_html_data[] = {
   "<HTML>\n"
   "<HEAD>\n"
   "<TITLE>159ers' Mottos</TITLE>\n"
   "</HEAD>\n"
   "<BODY>\n"
   "The few, the pround, semper fidelis!\n"
   "Failure has a best friend: loud excuses;\n"
   "success on the otherhand has only its own quiet work.\n"
   "</BODY></HTML>\n"
};
#define INDEX_HTML_SIZE ( sizeof( index_html_data ) ) // null is added+counted

char hello_html_data[] = {
   "<HTML><HEAD>\n"
   "<TITLE>Monkeys and Leopard</TITLE>\n"
   "</HEAD>\n"
   "<BODY BgColor=#FFFFFF>"
   "Monkeys in the jungle, taunting and mobbing a lone leopard passing by.\n"
   "They yelp and smug. Leopard hisses back -- a last warning.\n"
   "Monkeys however ignore it and acerbate their intimidation.\n"
   "Suddenly, Leopard strikes, in a lightning speed.\n"
   "Monkeys cries and flee, some slain and some seriously mauled.\n"
   "Primemates live in a testy social setting, in one another's eyes;\n"
   "but a leopard lives truely in nobody else's but its own eyes.\n"
   "Be a leopard at work, get things done; instead of one with big talks.\n"
   "</BODY></HTML>\n"
};
#define HELLO_HTML_SIZE ( sizeof( hello_html_data ) ) // null is added+counted

// We'll define "root_dir[]" later. Here is a forward declare.
extern dir_t root_dir[];                         // prototype it in advance

dir_t bin_dir[] = {
   { 16, MODE_DIR, ~0, ".", (char *)bin_dir },   // current dir
   { 17, MODE_DIR, ~0, "..", (char *)root_dir }, // parent dir, forward declared
   { 18, MODE_EXEC, SLEEP_SIZE, "sleep", (char *)sleep_bin },
   { 19, MODE_EXEC, HELLO_SIZE, "hello", (char *)hello_bin },
   {  0, 0, 0, NULL, NULL },                     // no entries in dir
   { END_DIR_INODE, 0, 0, NULL, NULL }           // end of bin_dir[]
};



dir_t www_dir[] = {
   { 10, MODE_DIR, ~0, ".", (char *)www_dir },
   { 11, MODE_DIR, ~0, "..", (char *)root_dir },
   { 12, MODE_FILE, HELLO_HTML_SIZE, "hello.html", (char *)hello_html_data },
   { 13, MODE_FILE, INDEX_HTML_SIZE, "index.html", (char *)index_html_data },
   { 14, MODE_FILE, HELLO_HTML_SIZE, "1 (hello.html)", (char *)hello_html_data },
   { 15, MODE_FILE, INDEX_HTML_SIZE, "2 (index.html)", (char *)index_html_data },
   {  0, 0, 0, NULL, NULL },          
   { END_DIR_INODE, 0, 0, NULL, NULL }
};

dir_t root_dir[] = {
   { 1, MODE_DIR, ~0, ".", (char *)root_dir },
   { 2, MODE_DIR, sizeof(bin_dir), "bin", (char *)bin_dir },
   { 3, MODE_DIR, sizeof(www_dir), "www", (char *)www_dir },
   { 4, MODE_FILE, HELP_TXT_SIZE, "help.txt", (char *)help_txt_data },
   { 5, MODE_FILE, NOTE_TXT_SIZE, "note.txt", (char *)note_txt_data },
   { 6, MODE_DIR, sizeof(bin_dir), "1 (bin dir)", (char *)bin_dir },
   { 7, MODE_DIR, sizeof(www_dir), "2 (www dir)", (char *)www_dir },
   { 8, MODE_FILE, HELP_TXT_SIZE, "3 (help.txt)", (char *)help_txt_data },
   { 9, MODE_FILE, NOTE_TXT_SIZE, "4 (note.txt)", (char *)note_txt_data },
   { 0, 0, 0, NULL, NULL },
   { END_DIR_INODE, 0, 0, NULL, NULL }
};

fd_t fd_array[MAX_FD];  // one file descriptor for every OPEN_OBJ call
// *********************************************************************

#endif

