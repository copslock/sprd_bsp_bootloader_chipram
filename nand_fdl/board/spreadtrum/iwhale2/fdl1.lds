
OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)
SECTIONS
{
	. = 0xE6000200;

	. = ALIGN(4);
	.text :
	{
		FDL_STARTDIR/fdl_start.o (.text*)
		*(.text*)
	}

	. = ALIGN(4);
	.rodata : { *(.rodata*) }

	. = ALIGN(4);
	.data : { *(.data*) }

    . = . +3k;
	. = ALIGN(4);
	__bss0_start = .;
	.bss0 :
	{
		*(.bss*)
	}
	__bss0_end = .;

	. = 0xE6022800;
	. = ALIGN(4);
	__bss1_start = .;
	.bss1 :
	{
		*(.bss*)
	}
	__bss1_end = .;
}
