/******************************************************************************
 *                        Signal processing file
 *
 * Description: This file contains routines for flash partition access which
 *              includes read and write.
 *              
 *		This file is a part of Electronic stethoscope project.
 *		You can find out more about the project from the README
 *			
 *
 * Author:      Vishwanath & Pavan
 * Repo:        github.com/vishwa91/e_steth
 * Date:        06-23-12
 ******************************************************************************/
#define READ_TO_BUFFER  0
#define READ_TO_DUMP    1
#define WRITE_TO_BUFFER 0
#define WRITE_TO_DUMP   1

void flash_erase(int *addr)
{
  _DINT();
  while(BUSY & FCTL3);                 // Check if Flash being used
  FCTL2 = FWKEY + FSSEL_1 + FN3;       // Clk = SMCLK/4
  FCTL1 = FWKEY + ERASE;               // Set Erase bit
  FCTL3 = FWKEY;                       // Clear Lock bit
  *addr = 0;                           // Dummy write to erase Flash segment
  while(BUSY & FCTL3);                 // Check if Flash being used
  FCTL1 = FWKEY;                       // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                // Set LOCK bit
  _EINT();
}
void flash_write(int *addr, char option)
{
  _DINT();
  int i = 0;
  FCTL2 = FWKEY + FSSEL_1 + FN0;       // Clk = SMCLK/4
  FCTL3 = FWKEY;                       // Clear Lock bit
  FCTL1 = FWKEY + WRT;                 // Set WRT bit for write operation
  
  if(option == WRITE_TO_BUFFER)
    for (i=0; i<64; i++)
      *addr++ = data_buffer[i];         // copy value to flash
  
  else if(option == WRITE_TO_DUMP)
    for (i=0; i<64; i++)
      *addr++ = data_dump[i];           // copy value to flash

  FCTL1 = FWKEY;                        // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                 // Set LOCK bit
  _EINT();
}

// This function is used for a block copy to RAM. Not necessacy in our program.
void flash_read(int *addr, char option)
{
  int i = 0;
  if(option == READ_TO_DUMP)
    for(i=0; i<64; i++)
      data_dump[i] = *addr++;
  else if(option == READ_TO_BUFFER)
    for(i=0; i<64; i++)
      data_buffer[i] = *addr++;
}