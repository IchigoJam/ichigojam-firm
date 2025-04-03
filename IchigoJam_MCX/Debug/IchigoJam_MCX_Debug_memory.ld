/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright 2008-2013 Code Red Technologies Ltd,
 * Copyright 2013-2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 * Generated linker script file for MCXA153
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v24.12 [Build 148] [2025-01-10] on 2025/04/03 16:53:05
 */

MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x20000 /* 128K bytes (alias Flash) */  
  SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x6000 /* 24K bytes (alias RAM) */  
  SRAMX0 (rwx) : ORIGIN = 0x4000000, LENGTH = 0x2000 /* 8K bytes (alias RAM2) */  
  SRAMX1 (rwx) : ORIGIN = 0x4002000, LENGTH = 0x1000 /* 4K bytes (alias RAM3) */  
}

  /* Define a symbol for the top of each memory region */
  __base_PROGRAM_FLASH = 0x0  ; /* PROGRAM_FLASH */  
  __base_Flash = 0x0 ; /* Flash */  
  __top_PROGRAM_FLASH = 0x0 + 0x20000 ; /* 128K bytes */  
  __top_Flash = 0x0 + 0x20000 ; /* 128K bytes */  
  __base_SRAM = 0x20000000  ; /* SRAM */  
  __base_RAM = 0x20000000 ; /* RAM */  
  __top_SRAM = 0x20000000 + 0x6000 ; /* 24K bytes */  
  __top_RAM = 0x20000000 + 0x6000 ; /* 24K bytes */  
  __base_SRAMX0 = 0x4000000  ; /* SRAMX0 */  
  __base_RAM2 = 0x4000000 ; /* RAM2 */  
  __top_SRAMX0 = 0x4000000 + 0x2000 ; /* 8K bytes */  
  __top_RAM2 = 0x4000000 + 0x2000 ; /* 8K bytes */  
  __base_SRAMX1 = 0x4002000  ; /* SRAMX1 */  
  __base_RAM3 = 0x4002000 ; /* RAM3 */  
  __top_SRAMX1 = 0x4002000 + 0x1000 ; /* 4K bytes */  
  __top_RAM3 = 0x4002000 + 0x1000 ; /* 4K bytes */  
