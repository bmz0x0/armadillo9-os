

OUTPUT_ARCH(arm)
ENTRY(stext)

SECTIONS
{
	. = 0xc0018000;

	__start = .;
	.init : {
		__init_begin = .;
		__inittext_begin = .;
		__sinittext = .;
		*(.init.text)
		__einittext = .;
		__inittext_end = .;
		__tagtable_begin = .;
		*(.taglist)
		__tagtable_end = .;
		. = ALIGN(16);
		*(.init.data)
		__init_end = .;
	}

	/DISCARD/ : {
	}

	.text : {
		__text_begin = .;
		*(.text)
		__text_end = .;
	}

	.rodata : AT(ADDR(.rodata) - 0) {
		*(.rodata)
	}

	__data_loc = .;
	.data : {
		*(.init.task)
		*(.data)
	}

	.rodata : {
		*(.rodata)
	}

	.bss : {
		__bss_start = .;
		*(.bss)
		__bss_end = .;
	}
	__end = .;

}


