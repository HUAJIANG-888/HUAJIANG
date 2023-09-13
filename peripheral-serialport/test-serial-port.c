#include <libserialport.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uart.h>

/* Example of how to send and receive data.
 *
 * This example file is released to the public domain. */

/* Helper function for error handling. */
int check(enum sp_return result);

int uart_led_init()
{

	/* Get the port names from the command line. */

	// if (argc < 2 || argc > 3) {
	// 	printf("Usage: %s </dev/ttyXXX> [</dev/ttyXXX>]\n", argv[0]);
	// 	return -1;
	// }
	

	/* The ports we will use. 
		将要用到的串口操作句柄*/
	

	/* Open and configure each port. */
	for (int i = 0; i < num_ports; i++) {
		printf("Looking for port %s.\n", port_names[i]);
		check(sp_get_port_by_name(port_names[i], &ports[i]));    //用户空间导入UART到代码空间中

		printf("Opening port.\n");
		check(sp_open(ports[i], SP_MODE_READ_WRITE));   //打开我们的设备节点

		printf("Setting port to 9600 8N1, no flow control.\n");
		check(sp_set_baudrate(ports[i], 9600));     //设置波特率
		check(sp_set_bits(ports[i], 8));					//设置数据位
		check(sp_set_parity(ports[i], SP_PARITY_NONE));				//设置校验位
		check(sp_set_stopbits(ports[i], 1));				//设置停止位
		check(sp_set_flowcontrol(ports[i], SP_FLOWCONTROL_NONE));   //设置流控
	}
		
}


int uart_send(char *dat){
		/* Now send some data on each port and receive it back. 
		现在开始发送与接收数据在串口中*/
	for (int tx = 0; tx < num_ports; tx++) {
		/* Get the ports to send and receive on. */
		int rx = num_ports == 1 ? 0 : ((tx == 0) ? 1 : 0);
		struct sp_port *tx_port = ports[tx];
		struct sp_port *rx_port = ports[rx];

		/* The data we will send. */
		char *data = dat;
		char ans = "ok";
		int size = strlen(data);

		/* We'll allow a 1 second timeout for send and receive. */
		unsigned int timeout = 1000;

		/* On success, sp_blocking_write() and sp_blocking_read()
		 * return the number of bytes sent/received before the
		 * timeout expired. We'll store that result here. */
		int result;

		/* Send data. */
		printf("Sending '%s' (%d bytes) on port %s.\n",
				data, size, sp_get_port_name(tx_port));			//得到串口名称
		result = check(sp_blocking_write(tx_port, data, size, timeout));

		/* Check whether we sent all of the data. */
		if (result == size)
			printf("Sent %d bytes successfully.\n", size);
		else
			printf("Timed out, %d/%d bytes sent.\n", result, size);

		// /* Allocate a buffer to receive data. */
		// char *buf = malloc(size + 1);

		// /* Try to receive the data on the other port. */
		// printf("Receiving %d bytes on port %s.\n",
		// 		size, sp_get_port_name(rx_port));
		// result = check(sp_blocking_read(rx_port, buf, size, timeout));

		// /* Check whether we received the number of bytes we wanted. */
		// if (result == size)
		// 	printf("Received %d bytes successfully.\n", size);
		// else
		// 	printf("Timed out, %d/%d bytes received.\n", result, size);

		// /* Check if we received the same data we sent. */
		// buf[result] = '\0';
		
		// printf("Received '%s'.\n", buf);

		// /* Free receive buffer. */
		//free(buf);
	}

	/* Close ports and free resources. */
	for (int i = 0; i < num_ports; i++) {
		check(sp_close(ports[i]));
		sp_free_port(ports[i]);
	}

	return 0;
	
}

/* Helper function for error handling. */
int check(enum sp_return result)
{
	/* For this example we'll just exit on any error by calling abort(). */
	char *error_message;

	switch (result) {
	case SP_ERR_ARG:
		printf("Error: Invalid argument.\n");
		abort();
	case SP_ERR_FAIL:
		error_message = sp_last_error_message();
		printf("Error: Failed: %s\n", error_message);
		sp_free_error_message(error_message);
		abort();
	case SP_ERR_SUPP:
		printf("Error: Not supported.\n");
		abort();
	case SP_ERR_MEM:
		printf("Error: Couldn't allocate memory.\n");
		abort();
	case SP_OK:
	default:
		return result;
	}
}


void main(){
	char *dat = "start\n";
	uart_led_init();
	uart_send(dat);
	return ;
}
