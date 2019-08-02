SECTIONS {
	.rodata : {
		rodata_start = .;
		*(.rodata);
	}
}
INSERT AFTER .text;

