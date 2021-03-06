#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

int connect_to_rf() {
	char *dest = readline("Bluetooth Address: ");
	char *channel = readline("Channel #:");
	struct sockaddr_rc addr = {0};
	int s, status;
	printf("Connecting to bluetooth device at [%s]\n", dest);
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t)atoi(channel);
	str2ba(dest, &addr.rc_bdaddr);
	status = connect(s, (const struct sockaddr *)&addr, sizeof(addr));
	if (status == 0) {
		printf("Sending hello message\n");
		// we have the connection!
		int bytes = write(s, "hello!\n", 7);
		printf("Sent [%d] bytes\n", bytes);
	} else {
		// conn failed
		perror("Connection failed");
	}
	close(s);
	return 0;
}

int main(int argc, char **argv) {
	inquiry_info *ii = NULL;
	int max_rsp, num_rsp;
	int dev_id, sock, len, flags;
	int i;
	char addr[19] = {0};
	char name[248] = {0};

	dev_id = hci_get_route(NULL);
	sock = hci_open_dev(dev_id);
	if (dev_id < 0 || sock < 0) {
		perror("opening socket");
		exit(1);
	}

	len = 8;
	max_rsp = 255;
	flags = IREQ_CACHE_FLUSH;
	ii = (inquiry_info *)malloc(max_rsp * sizeof(inquiry_info));
	printf("Scanning for nearby devices\n");

	num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
	if (num_rsp < 0)
		perror("hci_inquiry");

	for (i = 0; i < num_rsp; i++) {
		ba2str(&(ii + i)->bdaddr, addr);
		memset(name, 0, sizeof(name));
		if (hci_read_remote_name(sock, &(ii + i)->bdaddr, sizeof(name), name, 0) < 0)
			strcpy(name, "[unknown]");
		printf("%s  %s\n", addr, name);
	}

	free(ii);
	close(sock);
	connect_to_rf();

	return 0;
}
