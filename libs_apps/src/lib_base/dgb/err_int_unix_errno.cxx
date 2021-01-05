
#include "err_int.h"
#include "dgb.h"
#include "errno.h"
#include "buffer2.h"

// /usr/include/asm-generic
// 	errno-base.h
// 	errno.h

// cat /usr/include/asm-generic/errno-base.h
// cat /usr/include/asm-generic/errno.h

// :.,$ s,([0-9]+.*),// \1,
// :.,$ s,([0-9]+),// \1,
// :.,$ s,\([0-9]+.*\),// \1,
// :.,$ s,^.define.,MACRO(,
// :.,$ s,	,)	//	,
// :.,$ s,\t,)\t//\t,

#define OLD_DECL(SYMB,STRNG) \
	case SYMB: return STRNG;

#define DECL(SYMB,STRNG) \
	case SYMB: name = STRNG; break;

// : .,$ s_DECL(\([^,	"\([^")]*\)_DECL(\1,\t"\1"_

const char * get_name_of_errno( int e, buffer2 & buff ) {
// gdb_invoke();
 const char * name = NULL;
 buff.clear();
 switch( e ) {

#ifdef WIN32
	// names for errno - see ....
#else
DECL(EPERM,	"EPERM")	//	 1	/* Operation not permitted */
DECL(ENOENT,	"ENOENT")	//	 2	/* No such file or directory */
DECL(ESRCH,	"ESRCH")	//	 3	/* No such process */
DECL(EINTR,	"EINTR")	//	 4	/* Interrupted system call */
DECL(EIO,	"EIO")	//		 5	/* I/O error */
DECL(ENXIO,	"ENXIO")	//	 6	/* No such device or address */
DECL(E2BIG,	"E2BIG")	//	 7	/* Argument list too long */
DECL(ENOEXEC,	"ENOEXEC")	//	 8	/* Exec format error */
DECL(EBADF,	"EBADF")	//	 9	/* Bad file number */
DECL(ECHILD,	"ECHILD")	//	10	/* No child processes */
DECL(EAGAIN,	"EAGAIN")	//	11	/* Try again */
DECL(ENOMEM,	"ENOMEM")	//	12	/* Out of memory */
DECL(EACCES,	"EACCES")	//	13	/* Permission denied */
DECL(EFAULT,	"EFAULT")	//	14	/* Bad address */
DECL(ENOTBLK,	"ENOTBLK")	//	15	/* Block device required */
DECL(EBUSY,	"EBUSY")	//	16	/* Device or resource busy */
DECL(EEXIST,	"EEXIST")	//	17	/* File exists */
DECL(EXDEV,	"EXDEV")	//	18	/* Cross-device link */
DECL(ENODEV,	"ENODEV")	//	19	/* No such device */
DECL(ENOTDIR,	"ENOTDIR")	//	20	/* Not a directory */
DECL(EISDIR,	"EISDIR")	//	21	/* Is a directory */
DECL(EINVAL,	"EINVAL")	//	22	/* Invalid argument */
DECL(ENFILE,	"ENFILE")	//	23	/* File table overflow */
DECL(EMFILE,	"EMFILE")	//	24	/* Too many open files */
DECL(ENOTTY,	"ENOTTY")	//	25	/* Not a typewriter */
DECL(ETXTBSY,	"ETXTBSY")	//	26	/* Text file busy */
DECL(EFBIG,	"EFBIG")	//	27	/* File too large */
DECL(ENOSPC,	"ENOSPC")	//	28	/* No space left on device */
DECL(ESPIPE,	"ESPIPE")	//	29	/* Illegal seek */
DECL(EROFS,	"EROFS")	//	30	/* Read-only file system */
DECL(EMLINK,	"EMLINK")	//	31	/* Too many links */
DECL(EPIPE,	"EPIPE")	//	32	/* Broken pipe */
DECL(EDOM,	"EDOM")	//		33	/* Math argument out of domain of func */
DECL(ERANGE,	"ERANGE")	//	34	/* Math result not representable */

// cat /usr/include/asm-generic/errno-base.h
// cat /usr/include/asm-generic/errno.h

DECL(EDEADLK,	"EDEADLK")	//	35	/* Resource deadlock would occur */
DECL(ENAMETOOLONG,	"ENAMETOOLONG")	//	36	/* File name too long */
DECL(ENOLCK,	"ENOLCK")	//	37	/* No record locks available */
DECL(ENOSYS,	"ENOSYS")	//	38	/* Function not implemented */
DECL(ENOTEMPTY,	"ENOTEMPTY")	//	39	/* Directory not empty */
DECL(ELOOP,	"ELOOP")	//	40	/* Too many symbolic links encountered */
// DECL(EWOULDBLOCK,	"EWOULDBLOCK")	//	EAGAIN	/* Operation would block */
DECL(ENOMSG,	"ENOMSG")	//	42	/* No message of desired type */
DECL(EIDRM,	"EIDRM")	//	43	/* Identifier removed */
DECL(ECHRNG,	"ECHRNG")	//	44	/* Channel number out of range */
DECL(EL2NSYNC,	"EL2NSYNC")	//	45	/* Level 2 not synchronized */
DECL(EL3HLT,	"EL3HLT")	//	46	/* Level 3 halted */
DECL(EL3RST,	"EL3RST")	//	47	/* Level 3 reset */
DECL(ELNRNG,	"ELNRNG")	//	48	/* Link number out of range */
DECL(EUNATCH,	"EUNATCH")	//	49	/* Protocol driver not attached */
DECL(ENOCSI,	"ENOCSI")	//	50	/* No CSI structure available */
DECL(EL2HLT,	"EL2HLT")	//	51	/* Level 2 halted */
DECL(EBADE,	"EBADE")	//	52	/* Invalid exchange */
DECL(EBADR,	"EBADR")	//	53	/* Invalid request descriptor */
DECL(EXFULL,	"EXFULL")	//	54	/* Exchange full */
DECL(ENOANO,	"ENOANO")	//	55	/* No anode */
DECL(EBADRQC,	"EBADRQC")	//	56	/* Invalid request code */
DECL(EBADSLT,	"EBADSLT")	//	57	/* Invalid slot */

// DECL(EDEADLOCK,	"EDEADLOCK")	//	EDEADLK

DECL(EBFONT,	"EBFONT")	//	59	/* Bad font file format */
DECL(ENOSTR,	"ENOSTR")	//	60	/* Device not a stream */
DECL(ENODATA,	"ENODATA")	//	61	/* No data available */
DECL(ETIME,	"ETIME")	//	62	/* Timer expired */
DECL(ENOSR,	"ENOSR")	//	63	/* Out of streams resources */
DECL(ENONET,	"ENONET")	//	64	/* Machine is not on the network */
DECL(ENOPKG,	"ENOPKG")	//	65	/* Package not installed */
DECL(EREMOTE,	"EREMOTE")	//	66	/* Object is remote */
DECL(ENOLINK,	"ENOLINK")	//	67	/* Link has been severed */
DECL(EADV,	"EADV")	//		68	/* Advertise error */
DECL(ESRMNT,	"ESRMNT")	//	69	/* Srmount error */
DECL(ECOMM,	"ECOMM")	//	70	/* Communication error on send */
DECL(EPROTO,	"EPROTO")	//	71	/* Protocol error */
DECL(EMULTIHOP,	"EMULTIHOP")	//	72	/* Multihop attempted */
DECL(EDOTDOT,	"EDOTDOT")	//	73	/* RFS specific error */
DECL(EBADMSG,	"EBADMSG")	//	74	/* Not a data message */
DECL(EOVERFLOW,	"EOVERFLOW")	//	75	/* Value too large for defined data type */
DECL(ENOTUNIQ,	"ENOTUNIQ")	//	76	/* Name not unique on network */
DECL(EBADFD,	"EBADFD")	//	77	/* File descriptor in bad state */
DECL(EREMCHG,	"EREMCHG")	//	78	/* Remote address changed */
DECL(ELIBACC,	"ELIBACC")	//	79	/* Can not access a needed shared library */
DECL(ELIBBAD,	"ELIBBAD")	//	80	/* Accessing a corrupted shared library */
DECL(ELIBSCN,	"ELIBSCN")	//	81	/* .lib section in a.out corrupted */
DECL(ELIBMAX,	"ELIBMAX")	//	82	/* Attempting to link in too many shared libraries */
DECL(ELIBEXEC,	"ELIBEXEC")	//	83	/* Cannot exec a shared library directly */
DECL(EILSEQ,	"EILSEQ")	//	84	/* Illegal byte sequence */
DECL(ERESTART,	"ERESTART")	//	85	/* Interrupted system call should be restarted */
DECL(ESTRPIPE,	"ESTRPIPE")	//	86	/* Streams pipe error */
DECL(EUSERS,	"EUSERS")	//		87	/* Too many users */
DECL(ENOTSOCK,	"ENOTSOCK")	//	88	/* Socket operation on non-socket */
DECL(EDESTADDRREQ,	"EDESTADDRREQ")	//	89	/* Destination address required */
DECL(EMSGSIZE,	"EMSGSIZE")	//	90	/* Message too long */
DECL(EPROTOTYPE,	"EPROTOTYPE")	//	91	/* Protocol wrong type for socket */
DECL(ENOPROTOOPT,	"ENOPROTOOPT")	//	92	/* Protocol not available */
DECL(EPROTONOSUPPORT,	"EPROTONOSUPPORT")	//	93	/* Protocol not supported */
DECL(ESOCKTNOSUPPORT,	"ESOCKTNOSUPPORT")	//	94	/* Socket type not supported */
DECL(EOPNOTSUPP,	"EOPNOTSUPP")	//	95	/* Operation not supported on transport endpoint */
DECL(EPFNOSUPPORT,	"EPFNOSUPPORT")	//	96	/* Protocol family not supported */
DECL(EAFNOSUPPORT,	"EAFNOSUPPORT")	//	97	/* Address family not supported by protocol */
DECL(EADDRINUSE,	"EADDRINUSE")	//	98	/* Address already in use */
DECL(EADDRNOTAVAIL,	"EADDRNOTAVAIL")	//	99	/* Cannot assign requested address */
DECL(ENETDOWN,	"ENETDOWN")	//		100	/* Network is down */
DECL(ENETUNREACH,	"ENETUNREACH")	//	101	/* Network is unreachable */
DECL(ENETRESET,	"ENETRESET")	//	102	/* Network dropped connection because of reset */
DECL(ECONNABORTED,	"ECONNABORTED")	//	103	/* Software caused connection abort */
DECL(ECONNRESET,	"ECONNRESET")	//	104	/* Connection reset by peer */
DECL(ENOBUFS,	"ENOBUFS")	//	105	/* No buffer space available */
DECL(EISCONN,	"EISCONN")	//	106	/* Transport endpoint is already connected */
DECL(ENOTCONN,	"ENOTCONN")	//	107	/* Transport endpoint is not connected */
DECL(ESHUTDOWN,	"ESHUTDOWN")	//	108	/* Cannot send after transport endpoint shutdown */
DECL(ETOOMANYREFS,	"ETOOMANYREFS")	//	109	/* Too many references: cannot splice */
DECL(ETIMEDOUT,	"ETIMEDOUT")	//	110	/* Connection timed out */
DECL(ECONNREFUSED,	"ECONNREFUSED")	//	111	/* Connection refused */
DECL(EHOSTDOWN,	"EHOSTDOWN")	//	112	/* Host is down */
DECL(EHOSTUNREACH,	"EHOSTUNREACH")	//	113	/* No route to host */
DECL(EALREADY,	"EALREADY")	//		114	/* Operation already in progress */
DECL(EINPROGRESS,	"EINPROGRESS")	//	115	/* Operation now in progress */
DECL(ESTALE,	"ESTALE")	//	116	/* Stale NFS file handle */
DECL(EUCLEAN,	"EUCLEAN")	//	117	/* Structure needs cleaning */
DECL(ENOTNAM,	"ENOTNAM")	//	118	/* Not a XENIX named type file */
DECL(ENAVAIL,	"ENAVAIL")	//	119	/* No XENIX semaphores available */
DECL(EISNAM,	"EISNAM")	//	120	/* Is a named type file */
DECL(EREMOTEIO,	"EREMOTEIO")	//	121	/* Remote I/O error */
DECL(EDQUOT,	"EDQUOT")	//	122	/* Quota exceeded */

DECL(ENOMEDIUM,	"ENOMEDIUM")	//	123	/* No medium found */
DECL(EMEDIUMTYPE,	"EMEDIUMTYPE")	//	124	/* Wrong medium type */
DECL(ECANCELED,	"ECANCELED")	//	125	/* Operation Canceled */
#ifdef TGT_L24
// LINUX 2.4 does not have these numbers ?
#else
DECL(ENOKEY,	"ENOKEY")	//		126	/* Required key not available */
DECL(EKEYEXPIRED,	"EKEYEXPIRED")	//	127	/* Key has expired */
DECL(EKEYREVOKED,	"EKEYREVOKED")	//	128	/* Key has been revoked */
DECL(EKEYREJECTED,	"EKEYREJECTED")	//	129	/* Key was rejected by service */

/* for robust mutexes */
DECL(EOWNERDEAD,	"EOWNERDEAD")	//	130	/* Owner died */
DECL(ENOTRECOVERABLE,	"ENOTRECOVERABLE")	//	131	/* State not recoverable */

DECL(ERFKILL,	"ERFKILL")	//	132	/* Operation not possible due to RF-kill */
#endif
#endif

 default:
	
	WARN("errno %d not recognised", e );
	buff.print( "ERRNO%d", e );
	return (STR0) buff;
 }
 buff.set( name );
 return (STR0) buff;
}

