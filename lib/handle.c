/*
**  Copyright 1998-2003 University of Illinois Board of Trustees
**  Copyright 1998-2003 Mark D. Roth
**  All rights reserved.
**
**  handle.c - libtar code for initializing a TAR handle
**
**  Mark D. Roth <roth@uiuc.edu>
**  Campus Information Technologies and Educational Services
**  University of Illinois at Urbana-Champaign
*/

#include <internal.h>

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef STDC_HEADERS
# include <stdlib.h>
#endif


const char libtar_version[] = PACKAGE_VERSION;

static tartype_t default_type = { (openfunc_t)open, (closefunc_t)close, (readfunc_t)read, (writefunc_t)write };

static unsigned int dev_hash_wrap(void *p, unsigned int n)
{
	(void)n;
	return (unsigned int)dev_hash(p);
}

static int
tar_init(TAR **t, const char *pathname, tartype_t *type,
	 int oflags, int mode, int options)
{
	if ((oflags & O_ACCMODE) == O_RDWR)
	{
		errno = EINVAL;
		return -1;
	}

	*t = (TAR *)calloc(1, sizeof(TAR));
	if (*t == NULL)
		return -1;

	(*t)->pathname = pathname;
	(*t)->options = options;
	(*t)->type = (type ? type : &default_type);
	(*t)->oflags = oflags;

	if ((oflags & O_ACCMODE) == O_RDONLY)
		(*t)->h = libtar_hash_new(256,
					  (libtar_hashfunc_t)path_hashfunc);
	else
		(*t)->h = libtar_hash_new(16, dev_hash_wrap);
	if ((*t)->h == NULL)
	{
		free(*t);
		*t = NULL;
		return -1;
	}

	return 0;
}


/* open a new tarfile handle */
int
tar_open(TAR **t, const char *pathname, tartype_t *type,
	 int oflags, int mode, int options)
{
	if (tar_init(t, pathname, type, oflags, mode, options) == -1)
		return -1;

	if ((options & TAR_NOOVERWRITE) && (oflags & O_CREAT))
		oflags |= O_EXCL;

#ifdef O_BINARY
	oflags |= O_BINARY;
#endif

	(*t)->fd = (*((*t)->type->openfunc))(pathname, oflags, mode);
	if ((*t)->fd == (void*)-1)
	{
		libtar_hash_free((*t)->h, NULL);
		free(*t);
		*t = NULL;
		return -1;
	}

	return 0;
}


int
tar_fdopen(TAR **t, int fd, const char *pathname, tartype_t *type,
	   int oflags, int mode, int options)
{
	if (tar_init(t, pathname, type, oflags, mode, options) == -1)
		return -1;

	(*t)->fd = (void*)(uint64_t)fd;
	return 0;
}

int
tar_fdpopen(TAR **t, void *fdp, const char *pathname, tartype_t *type,
	   int oflags, int mode, int options)
{
	if (tar_init(t, pathname, type, oflags, mode, options) == -1)
		return -1;

	(*t)->fd = fdp;
	return 0;
}


int tar_fd(TAR *t){
	return (int)(uint64_t)t->fd;
}

void *tar_fdp(TAR *t){
	return t->fd;
}


/* close tarfile handle */
int
tar_close(TAR *t)
{
	int i;

	i = (*(t->type->closefunc))(t->fd);

	if (t->h != NULL)
		libtar_hash_free(t->h, ((t->oflags & O_ACCMODE) == O_RDONLY
					? free
					: (libtar_freefunc_t)tar_dev_free));
	if (t->th_pathname != NULL)
		free(t->th_pathname);
	free(t);

	return i;
}
