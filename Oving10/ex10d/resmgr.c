#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#include "fifo.h"

dispatch_t              *dpp;
resmgr_attr_t           resmgr_attr;
dispatch_context_t      *ctp;
resmgr_connect_funcs_t  connect_funcs;
resmgr_io_funcs_t       io_funcs;
iofunc_attr_t           io_attr;



int io_read(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb);
int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
void* counter_manipulator();

fifo_t queue;

pthread_mutex_t queue_mut	= PTHREAD_MUTEX_INITIALIZER;



void error(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	printf("Start resource manager\n");

	// create dispatch.
	if (!(dpp = dispatch_create()))
		error("Create dispatch");

	// initialize resource manager attributes.
	memset(&resmgr_attr, 0, sizeof(resmgr_attr));
	resmgr_attr.nparts_max = 1;
	resmgr_attr.msg_max_size = 2048;

	// set standard connect and io functions.
	iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs, _RESMGR_IO_NFUNCS, &io_funcs);
	iofunc_attr_init(&io_attr, S_IFNAM | 0666, 0, 0);

	// override functions
	io_funcs.read = io_read;
	io_funcs.write = io_write;
	//io_funcs.write = io_write;

	// establish resource manager
	if (resmgr_attach(dpp, &resmgr_attr, "/dev/myresource", _FTYPE_ANY, 0, &connect_funcs, &io_funcs, &io_attr) < 0)
		error("Resmanager attach");
	ctp = dispatch_context_alloc(dpp);

	pthread_mutex_lock(&queue_mut);
	fifo_init(&queue);
	pthread_mutex_unlock(&queue_mut);

	// wait forever, handling messages.
	while(1)
	{
		if (!(ctp = dispatch_block(ctp)))
			error("Dispatch block");
		dispatch_handler(ctp);
	}

	exit(EXIT_SUCCESS);
}

int io_read(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb){
	char buf[WIDTH] = "";
	if(!ocb->offset){
		// set data to return
		pthread_mutex_lock(&queue_mut);
		fifo_rem_string(&queue, &buf);
		pthread_mutex_unlock(&queue_mut);
		if(buf != ""){
			SETIOV(ctp->iov, buf, strlen(buf));
			_IO_SET_READ_NBYTES(ctp, strlen(buf));

			// increase the offset (new reads will not get the same data)
			ocb->offset = 1;

			// return
			return (_RESMGR_NPARTS(1));
		}
	} else {
		// set to return no data
		_IO_SET_READ_NBYTES(ctp, 0);

		// return
		return (_RESMGR_NPARTS(0));
	}
}

int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb){
	char    buf[WIDTH];
	msg->i.nbytes = WIDTH;

	/* set up the number of bytes (returned by client's write()) */
	_IO_SET_WRITE_NBYTES (ctp, msg->i.nbytes);

	/*
	 *  Reread the data from the sender's message buffer.
	 *  We're not assuming that all of the data fit into the
	 *  resource manager library's receive buffer.
	 */
	resmgr_msgread(ctp, buf, msg->i.nbytes, sizeof(msg->i));
	buf [msg->i.nbytes] = '\0'; /* just in case the text is not NULL terminated */
	SETIOV(ctp->iov, buf, strlen(buf));
	_IO_SET_READ_NBYTES(ctp, strlen(buf));


	pthread_mutex_lock(&queue_mut);
	fifo_add_string(&queue, buf);
	pthread_mutex_unlock(&queue_mut);

	return (_RESMGR_NPARTS (0));
}

