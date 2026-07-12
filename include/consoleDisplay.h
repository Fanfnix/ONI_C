#ifndef CONSOLE_DISPLAY
#define CONSOLE_DISPLAY

typedef struct {
    const char *id;
    char ascii_char;
} ElementAsciiMapping;

char element_ascii_char(const char *element_id);

#endif