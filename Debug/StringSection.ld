SECTIONS {
	.logdebug : {
		logdebug_start = .;
		*(.logdebug);
	}
}
INSERT AFTER .text;

