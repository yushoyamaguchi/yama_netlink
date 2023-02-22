#include "libnetlink.h"


void hexdump(const void *buffer, size_t bufferlen)
{
  const unsigned char *data = (const unsigned char *)(buffer); 
  for (size_t i = 0; i < bufferlen; i++) {
    if (i % 4 == 0) {
      printf("  ");
    }
    if (i % 16 == 0) {
      printf("  \n");
    }
    printf("%02X", data[i]);
  }
}

int addattr_l(struct nlmsghdr *n, int maxlen,
    int type, const void *data, int alen)
{
  int len = RTA_LENGTH(alen);
  struct rtattr *rta;

  if (NLMSG_ALIGN(n->nlmsg_len) + RTA_ALIGN(len) > maxlen) {
    fprintf(stderr,
      "addattr_l ERROR: message exceeded bound of %d\n",
      maxlen);
    return -1;
  }

  rta = NLMSG_TAIL(n);
  rta->rta_type = type;
  rta->rta_len = len;
  if (alen)
    memcpy(RTA_DATA(rta), data, alen);
  n->nlmsg_len = NLMSG_ALIGN(n->nlmsg_len) + RTA_ALIGN(len);
  return 0;
}

int addattr32(struct nlmsghdr *n, int maxlen, int type, __u32 data)
{
    return addattr_l(n, maxlen, type, &data, sizeof(__u32));
}

int nl_talk_iov(int fd, struct iovec *iov)
{
  struct sockaddr_nl nladdr = { .nl_family = AF_NETLINK };
    struct msghdr msg = {
        .msg_name = &nladdr,
        .msg_namelen = sizeof(nladdr),
        .msg_iov = iov,
        .msg_iovlen = 1,
    };

    sendmsg(fd, &msg, 0);

    char recv_buf[MAX_RECV_BUF_LEN];

    struct iovec riov = {
        .iov_base = &recv_buf,
        .iov_len = MAX_RECV_BUF_LEN};

    msg.msg_iov = &riov;
    msg.msg_iovlen = 1;

    int recv_len = recvmsg(fd, &msg, 0);
    if (recv_len < 0)
    {
        perror("Receiving message failed");
    }

    struct nlmsghdr *rnh;

    for (rnh = (struct nlmsghdr *)recv_buf; NLMSG_OK(rnh, recv_len); rnh = NLMSG_NEXT(rnh, recv_len))
    {

        if (rnh->nlmsg_type == NLMSG_ERROR)
        {
            struct nlmsgerr *errmsg;
            errmsg = NLMSG_DATA(rnh);
            printf("%d, %s\n", errmsg->error, strerror(-errmsg->error));
            break;
        }
    }

    return 1;
}
