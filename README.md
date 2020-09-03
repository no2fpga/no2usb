Nitro USB core
==============

This fpga core implements a USB Full-Speed SIU using only normal FPGA CMOS IOs.

The main design goals are:

* Small and efficient (originaly targetting ice40)
* Flexible, even at runtime (dynamic EP config)
* Constant size no matter how many EPs are use
* Single clock-domain (currently 48M but soon extended to any 12M
  multiple starting at 36M).


Limitations
-----------

The current code in this repo targets the iCE40 only and uses some direct
`SB_IO` and `SB_RAM_4K` instances. A more generic version will be added soon,
it has already been run on ECP5 for the Hack-a-day badge 2019 supercon but
the small tweaks haven't been added here yet.


License
-------

See LICENSE.md for the licenses of the various components in this repository
