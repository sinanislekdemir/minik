#ifndef _kernel_eeprom_hpp
#define _kernel_eeprom_hpp

#define EEPROM_SIZE 1024 * 4
#define STX 2
#define ETX 3

int init_eeprom();
int eeprom_free_area();
int eeprom_clean();
int eeprom_start_program();
int eeprom_write_line(const char *buffer);
int eeprom_read_line(char *back);
int eeprom_end_program();
int eeprom_program_count();

#endif
