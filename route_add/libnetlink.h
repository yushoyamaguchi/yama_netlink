#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/socket.h>
#include <asm/types.h>
#include <arpa/inet.h>

#include <net/if.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#define MAX_RECV_BUF_LEN 4096

#define NLMSG_TAIL(nmsg) \
  ((struct rtattr *) (((void *) (nmsg)) + NLMSG_ALIGN((nmsg)->nlmsg_len)))

int addattr_l(struct nlmsghdr *n, int maxlen, int type, const void *data, int alen);
int addattr32(struct nlmsghdr *n, int maxlen, int type, __u32 data);

int nl_talk_iov(int fd, struct iovec *iov);

void hexdump(const void *buffer, size_t bufferlen);