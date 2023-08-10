<!-- doc_parts/00_disk_action -->
<!DOCTYPE HTML SYSTEM [
	<!entity c1t1d0 sdata "c2t2d0">
	<!entity devno1 sdata "8/8.8.0">
]>
<HTML>
<HEAD>
<TITLE> Mirror Disk Action ££££ </TITLE>
</HEAD>
<BODY bgcolor="#FFFFFF">

<!-- doc_parts/01_lead_in -->
<h4> DR Exercise - Mirrored Disks </h4><p>

	Here is a document I wrote one evening for a friend to use
	in their Disaster Recovery Weekend Docs as a L1 section.

	Now I have left my customers, I need some ££££
</>

<DIV class="TOC"> 
	this gets deleted
	And replaced with the TOC

	You can enhance the script to do RE-numbering, by removing
	and numbers found in headers, particularly in A-NAME.
</>

<H1> Test Boot - Mirrored Disk </H1><p>

	With a pair of mirrored root disks, you have some fail-over
	reliability.  If one fails, the other carries on. If you dont boot,
	HPUX knows which is the good one, and that it <B>is</B> good.

</P>




<!-- doc_parts/10_find_which -->
<H2> Finding which disk is which </H2><p>
	You dont want to pull the wrong one!

</P><H3> showboot </H3><p>
	On the running system, run 'showboot' to see what is set
	as the PRIMARY path, and what is probably the ALT-PATH (not
	necessarily). Use ioscan -kfn to convert numbers into device names.
</P><P>
	Dont worry, if you ever had to do this, the HP engineer would do
	it at the hardware level.

</P><H3> ioscan -kfn </H3><p>

	On the running system, (or from an archived printout), obtain
	the output from <code>ioscan -kfn</code>. It helps you convert
	device names to and from device numbers.

</P><H3> Disk Activity LED </H3><p>
	Supposing &c1t1d0; &nbsp; <!-- bug space after sdata migrated away
	--> is the primary path, you want to know which disk that is.
	Simply READ a lot from the disk, and see which light comes on,
	and label it with a sticky tab. Be certain though.
<PRE>
dd if=/dev/&c1t1d0; of=/dev/null
</PRE>

<H3> Repeat for Secondary </H3><p>
	There is no need to do this, as you shall only remove the primary.
</P><P>
	It is worth remembering, that the entire disk is NOT primary
	or secondary, but that each part of the first disk is repeated
	or mirrored on the second disk. We call them that for sanity,
	but HPUX has no such concept. It has a selector variable to
	pick a disk to boot from, and LVM mirroring.

</P><H3> Find an LVOL </H3><p>
	If you follow this test script, (on a different system), you
	will need to know the name of an LVOL, so that you can check the
	status of its PE's (Physical Extent = 4 M track, with independent
	versioning).
</P><P>
	You may wish to get a list of all the LVOL's on the disk that
	you are removing (and also check everything on that PV is mirrored!)
</P><P>
	vgdisplay -v /dev/vg00 will show you a list.

</P>

<!-- doc_parts/20_run_with_one -->
<H2> Running with one disk </H2><p>

</P><H3> Check for (no) messages </H3><p>
	Run lvdisplay on a mirrored LVOL (on the disk you are looking at),
	and check that it says 'current' on both disks. Check syslog.

</P><H3> Remove either disk </H3><p>
	You can ONLY remove a disk if it is 'hot-pluggable'. BOX5 has
	its disks in the external Jamaica cabinets, with those grey/blue
	levers to pull and reseat disks.
</P><P>
	Now remove one of the two disks, by pulling it out by 1/2"

</P><H3> POWERFAIL messages </H3><p>
	Check syslog, dmesg and lvdisplay. You should see error messages,
	saying POWERFAIL (on the disk address) but the system still runs.
	lvdisplay should show errors.

</P><H3> Replace Disk </H3><p>
	Now firmly/gently put the disk back, and check syslog/dmesg until
	you see the POWERFAIL/RECOVERED message. That indicates that
	the disk has been seen and checked by the OS, as being there and
	functioning correctly.

</P><H3> PE becomes CURRENT </H3><p>
	Quickly check lvdisplay (rerunning several times) and you will
	see some physical extents change from ERROR/STALE to current.
	That is the LVM bringing both sides up-to-date.

</P><H3> Now Normal </H3><p>
	When that is finished, the system is fully back to normal.

</P>
<!-- doc_parts/30_boot_with_one -->
<H2> Booting with one disk </H2><p>

</P>
<!-- doc_parts/31_quorum_boot_check -->

<H3> Quorum check at boot time </><p>
	If you reboot, with a disk missing, HPUX cannot be certain that
	the disk that works, has the proper data on it, so the LVM refuses
	to activate the LV's. When you boot via the alternate path, it
	suceeds in finding, loading and running the kernel, but when it
	then activates LVM, it fails.
</P><P>
	If you had three equally mirrored disks (what HP calls two
	mirror copies), and two disks vouched for each-other (ie showed the
	same data revisions), then HPUX would believe the two, and boot
	without the third. HPUX calls that quorum (more than half).
</P><P>
	Since you have two equally mirrored disks (one mirror copy), when one
	is down, the system wont boot, unless you tell it to avoid the
	quorum check (using hpux -lq).
</P><P>
	It is worth trying to see what happens when you dont type anything
	(ie dont boot with hpux -lq), and it only takes 5 minutes, as a
	full reboot is not necessary, it quickly drops back into IPL mode.
</P>

<!-- doc_parts/32_alt_boot_path -->

<H3> Alternate Boot Path </><p>
	As well as two mirrors, (of each of the LVOLS), there are two
	boot tracks. If you boot without the alternate disk, hpux might
	not notice until it tries to actives the LVM. If you boot without
	the primary path, hpux will notice straight away.
</P><p>
	Find out which is the primary path by running 'showboot'. Find
	out which is that physical disk by reading from it and checking
	the LED. (See above).
</P>

<!-- doc_parts/33_shutdown_h -->

<H3> shutdown -h </H3><p>
	Shutdown the machine, using shutdown -h, and remove the primary
	path disk, and boot. Booting takes at least 20 minutes, because
	the system wants to check everything.
</P>

<H3> let autoboot fail </H3><p>
	First let it try its own thing, to see what happens. It will
	detect the missing pri-path disk and should return to the BIOS
	command line.
</P>

<!-- doc_parts/36_bios_bits -->

<H3> BIOS Prompt </H3><p>
	To get here without removing that disk, press the space-bar
	during booting, when it says Press-Any-Key within 10 seconds.
	Options include:

</P> <PRE>

HELP
MENU
</PRE> <P>

	Its important to realise that there are two levels. There is the
	machine BIOS, which does not come from any disk, and there is an
	IPL-BIOS which comes from the disk. You have not read the disk yet.
	(Which is just as well, since you have removed it/one of them).
</P><p>
	You can set the PRI and ALT PAths from here, or you can leave
	them and boot from a named (numbered) path. You can also do that
	from the second stage loader, or from UNIX.

</P>

<H3> BIOS Commands </H3><p>
	<CODE>SEA</CODE> or 'search', will tell you the list of devices
	which the bios allows you to attempt to boot from. A recent copy
	of ioscan helps, so that you KNOW which is the tape, cdrom or
	disk. To boot an ignite tape, boot from that tape device.
</P><p>
	(DO NOT RUN YET) <CODE>BO ALT</CODE> or 'boot alternate', will
	attempt to boot from the configured alternative path.  <CODE>BO
	&devno1;</CODE> will attempt to boot from that SCSI controller,
	that LUN device. (Dont do that, now, unless you want to)

</P><H3> ALT &devno1; </H3><p>
	The setting of the ALT PAth is not important, and may be wrong.
	You can change it if you wish, from here, from the IPL prompt
	or from a running UNIX.

</P>

<H3> BO </H3><p>
	<CODE>BO</CODE> or 'boot', will probably offer you a choice of
	PRI/ALT bootable paths (disks), and also a chance to 'interact
	with the IPL'. Say 'Y' if you want to specify the -lq option later.
	If you said <CODE>BO ALT</CODE>, you wont see the first two options.

</P><PRE>
Boot from primary - N
Boot from alternate - Y
Interact with IPL? - Y
</PRE>

<H3> ISL&gt; IPL Commands </H3><p>
	That leaves you in the ISL BIOS, loaded from the disk. Again try
	HELP and MENU.

	<CODE>hpux show autofile</CODE> is like showboot, but from BIOS
	not from UNIX. If the ALTPATH is already set, life is a tiny bit
	easier, but it is often set to the tape (which you want to boot
	from for ignite), or the CDROM (which you want to reinstall).
	Commands include:

</P> <PRE>
help

hpux		   # to boot as normally
hpux show autofile # like lifcp to screen
hpux -is boot      # boot to single user state
hpux -lq           # boot without quorum check
hpux -lm           # 3-38 - NO SWAP NO LVM # AVOID 
hpux /stand/vmunix.BCKUP # for the old kernel 
hpux .... 	   # see 2-7 for other kernels
hpux ....          # also combinations of options
primpath &devno1;  # permanently use different boot path
</PRE>
 <p>

	DO NOT RUN: hpux -lm It will take you to maintenance mode, but
	you will have to reboot, so its mostly for when you think you
	can recover the disk and immediately reboot.
</P><p>
	<CODE>hpux -lq</CODE> will boot with no quorum check, which will
	get your entire machine working. You can also add the <b>-is</b>
	to go into single-user state, then do an <b>init 4</b>, but that
	might be confusing if you have no reason to do so.
</P><p>
	If you have a second machine, up and running, try 'man hpux'
</P><p>
	Why is ISL also called IPL ?  The Initial Program Loader, loads
	the Initial System Loader from disk, and then runs the ISL.
	Its similar to the way HP-CDROMS have one printed label on the
	media, and another label on the carton.

</P>

<H3> hpux -lq </H3><p>
	Boot using 'hpux -lq', and the machine will come up cleanly
	(presuming that disk was the only one missing, and other LVOLs
	wont have difficulties). Otherwise add the '-is' option and figure
	it from there.
</P><p>
	There will still be messages about the disk not being there,
	but the other mirrored parts make it non-fatal.

</P>

<!-- doc_parts/38_coming_up -->

<H3> System Now Up  -- on one disk </H3><p>
	Quickly test that Informix is functioning at all, check syslog.

</P>

<H3> Replace Primary Disk </H3><p>
	Now return the missing disk, by gently pushing the lever (over
	a catch). The LED will blink, and there may even be a SCSI bus
	reset as the controller detects it.

</P>

<H3> ioscan detect disk </H3><p>
	HPUX will not know its back, or even that it exists! You need
	to run <CODE>ioscan</CODE>, without the -k option (from kernel
	memory), but probably with -fn and through pg.
</P><p>
	The HPUX kernel now knows that the device exists, but the LVM
	is still running without it. (I'm presuming the replacement
	disk is the original disk, with the old LVM info on it. If not,
	you will need to check vgcfgrestore in the manual pages and the
	Admin-Tasks-Guide -- again -- if that ever really happens the
	HP engineer will do that with you).

</P>

<H3> vgsync </H3><p>
	<CODE>vgsync</CODE> is required to tell the LVM to look-for,
	find and re-sync with the disk. Notice that you didn't do that
	when you didn't boot. You must do it this time, because the LVM
	has completely forgotten about the absent disk, and is not polling
	to find it.

</P>

<H3> stale/error becomes current </H3><p>
	<CODE>lvdisplay -v /dev/vg00/lvol11</CODE> will show you lots
	of physical extents in error or stale, but repeated running will
	show you the LVM bringing each PE back into CURRENT status.

</P>

<H3> System Now Up - on both mirrors </H3><p>
	You can now proceed, or if superstitious, you can do a full
	normal, unattended reboot, to be sure.
</P>


