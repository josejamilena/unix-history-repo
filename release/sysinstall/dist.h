#ifndef _DIST_H_INCLUDE
#define _DIST_H_INCLUDE

/* Bitfields for distributions - hope we never have more than 32! :-) */
#define DIST_BIN		0x0001
#define DIST_GAMES		0x0002
#define DIST_MANPAGES		0x0004
#define DIST_PROFLIBS		0x0008
#define DIST_DICT		0x0010
#define DIST_SRC		0x0020
#define DIST_DOC		0x0040
#define DIST_INFO		0x0080
#define DIST_COMPAT1X		0x0100
#define DIST_COMPAT20		0x0200
#define DIST_COMPAT21		0x0400
#define DIST_XF86		0x0800
#define DIST_COMMERCIAL		0x1000
#define DIST_DES		0x2000
#define DIST_EXPERIMENTAL	0x4000
#define DIST_ALL		0x0FFF	/* Don't include commerce, DES or experimental in "all" */

/* Canned distribution sets */
#define _DIST_DEVELOPER \
	(DIST_BIN | DIST_DOC | DIST_MANPAGES | DIST_DICT | DIST_PROFLIBS | DIST_INFO | DIST_SRC)

#define _DIST_USER \
	(DIST_BIN | DIST_DOC | DIST_MANPAGES | DIST_DICT | DIST_COMPAT1X | DIST_COMPAT20)

/* Subtypes for DES distribution */
#define DIST_DES_DES		0x0001
#define DIST_DES_SEBONES	0x0002
#define DIST_DES_SSECURE	0x0004
#define DIST_DES_KERBEROS	0x0008

/* Subtypes for SRC distribution */
#define DIST_SRC_BASE		0x0001
#define DIST_SRC_GNU		0x0002
#define DIST_SRC_ETC		0x0004
#define DIST_SRC_GAMES		0x0008
#define DIST_SRC_INCLUDE	0x0010
#define DIST_SRC_LIB		0x0020
#define DIST_SRC_LIBEXEC	0x0040
#define DIST_SRC_LKM		0x0080
#define DIST_SRC_RELEASE	0x0100
#define DIST_SRC_SBIN		0x0200
#define DIST_SRC_SHARE		0x0400
#define DIST_SRC_SYS		0x0800
#define DIST_SRC_UBIN		0x1000
#define DIST_SRC_USBIN		0x2000
#define DIST_SRC_BIN		0x4000
#define DIST_SRC_SMAILCF	0x8000
#define DIST_SRC_ALL		0x7FFF	/* no SMAILCF, it's part of USBIN */

/* Subtypes for XFree86 distribution */
#define DIST_XF86_BIN		0x0001
#define DIST_XF86_LIB		0x0002
#define DIST_XF86_DOC		0x0004
#define DIST_XF86_ETC		0x0008
#define DIST_XF86_MAN		0x0010
#define DIST_XF86_CTRB		0x0020
#define DIST_XF86_PROG		0x0040
#define DIST_XF86_LINK		0x0080
#define DIST_XF86_UBIN		0x0100
#define DIST_XF86_PEX		0x0200
#define DIST_XF86_LBX		0x0400
#define DIST_XF86_XINIT		0x0800
#define DIST_XF86_XDMCF		0x1000
#define DIST_XF86_SRC		0x2000
#define DIST_XF86_MISC_ALL	0x3FFF
#define DIST_XF86_SERVER	0x4000
#define DIST_XF86_SERVER_8514		0x0001
#define DIST_XF86_SERVER_AGX		0x0002
#define DIST_XF86_SERVER_MACH8		0x0004
#define DIST_XF86_SERVER_MACH32		0x0008
#define DIST_XF86_SERVER_MACH64		0x0010
#define DIST_XF86_SERVER_MONO		0x0020
#define DIST_XF86_SERVER_P9000		0x0040
#define DIST_XF86_SERVER_S3		0x0080
#define DIST_XF86_SERVER_SVGA		0x0100
#define DIST_XF86_SERVER_VGA16		0x0200
#define DIST_XF86_SERVER_W32		0x0400
#define DIST_XF86_SERVER_NEST		0x0800
#define DIST_XF86_SERVER_ALL		0x0FFF
#define DIST_XF86_FONTS		0x8000
#define DIST_XF86_FONTS_MISC		0x0001
#define DIST_XF86_FONTS_100		0x0002
#define DIST_XF86_FONTS_CYR		0x0004
#define DIST_XF86_FONTS_SCALE		0x0008
#define DIST_XF86_FONTS_NON		0x0010
#define DIST_XF86_FONTS_SERVER		0x0020
#define DIST_XF86_FONTS_ALL		0x003F
#define DIST_XF86_ALL		0xFFFF

#endif
/* _DIST_H_INCLUDE */
