/* Header for poll(2) emulation
   Contributed by Paolo Bonzini.

   Copyright 2001, 2002, 2003, 2007, 2009, 2010 Free Software Foundation, Inc.

   This file is part of gnulib.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifndef _POLL_STRUCTURES_H
#define _POLL_STRUCTURES_H

/* fake a poll(2) environment */
#define POLLIN      0x0001      /* any readable data available   */
#define POLLPRI     0x0002      /* OOB/Urgent readable data      */
#define POLLOUT     0x0004      /* file descriptor is writeable  */
#define POLLERR     0x0008      /* some poll error occurred      */
#define POLLHUP     0x0010      /* file descriptor was "hung up" */
#define POLLNVAL    0x0020      /* requested events "invalid"    */
#define POLLRDNORM  0x0040
#define POLLRDBAND  0x0080
#define POLLWRNORM  0x0100
#define POLLWRBAND  0x0200

#ifdef __cplusplus
extern "C" {
#endif


	struct commonapi_pollfd
	{
		unsigned int fd;                       /* which file descriptor to poll */
		short events;                 /* events we are interested in   */
		short revents;                /* events found on return        */
	};

	typedef unsigned long nfds_t;

	//extern int poll(struct commonapi_pollfd *pfd, nfds_t nfd, int timeout);

#ifdef __cplusplus
}
#endif

#endif /* _POLL_STRUCTURES_H */
