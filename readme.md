Repository for poxim firmware



getting started developing:

	basically this:
	https://justanotherelectronicsblog.com/?p=432

	notes on installation
		make sure you have texinfo & lib-usb dev packages:
			sudo apt-get update -y
			sudo apt-get install -y texinfo libusb-1.0-0-dev

		open ocd apt-get does not include support, compile from latest sources:
			git clone --recurse-submodule git://git.code.sf.net/p/openocd/code openocd-code
			follow readme instructions:
				./bootstrap
				./configure [options]
				make
				sudo make install
			N.B. to use an stlink device to connect to a target add "--enable-stlink" as option, Personally I compiled with options:
				"--enable-maintainer-mode --enable-ftdi --enable-stlink"	but just stlink should work too.

			for the included eclipse executable to work export the location of this openocd install as OPENOCD_DEV variable and the location of the scripts folder as OPENOCD_SCRIPTS in ~/.bashrc . 
				eg: export OPENOCD_DEV=/usr/local/bin/openocd
					export OPENOCD_SCRIPTS=/usr/share/openocd/scripts

		In eclipse go to window -> preferences -> C/C++ -> Code Analysis
			Disable everything => you could leave some of them in but there are code structures illegal in normal coding but fine for embedded that cause errors and make you unable to compile the code


	notes on programming:
		suggest reading the sdcc manual, most important stuf for basic use in chapter 3.

		STM8S_StdPeriph_Driver/inc/stm8s_itc.h line :44-115 contain irq numbers connectocted to their respective name using enum:
		inc/stm8s_it.h : contains the names to use for interrupts.
		use them like:
			INTERRUPT_HANDLER('name', 'number or enum name'){

			}
		they can be in any file however it is preferred to keep them in main.
		