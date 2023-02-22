#include "libnetlink.h"

#define DST_MASK_LEN 32


int ipv4_route_add(struct in_addr src_addr, struct in_addr dst_addr, uint32_t index) {

  struct netlink_msg{
    struct nlmsghdr n;
    struct rtmsg r;
    char buf[4096];
  };  

  struct netlink_msg req;

  int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

  if (fd < 0) {
    return 0;
  }

  req.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
  req.n.nlmsg_flags = NLM_F_REQUEST | NLM_F_CREATE | NLM_F_ACK | NLM_F_REPLACE;
  req.n.nlmsg_type  = RTM_NEWROUTE;
  req.r.rtm_family = AF_INET;
  req.r.rtm_dst_len = DST_MASK_LEN;
  req.r.rtm_src_len = 0;
  req.r.rtm_tos = 0;
  req.r.rtm_table = RT_TABLE_MAIN; // 0xFE
  req.r.rtm_protocol = RTPROT_STATIC; //0x04
  req.r.rtm_scope = RT_SCOPE_UNIVERSE; // 0x00
  req.r.rtm_type = RTN_UNICAST; // 0x01
  req.r.rtm_flags = 0;
 
  addattr_l(&req.n, sizeof(req), RTA_DST, &src_addr, sizeof(struct in_addr));
  addattr_l(&req.n, sizeof(req),
          RTA_GATEWAY, &dst_addr,
          sizeof(struct in_addr));

  uint32_t oif_idx = index;
  addattr32(&req.n, sizeof(req), RTA_OIF, oif_idx);

  struct iovec iov = {&req, req.n.nlmsg_len };

  nl_talk_iov(fd, &iov);
  return 1; 

}

int main(int argc, char *argv[]) {

  char *cmd_ip_str;
  char *cmd_via_ip_str;
  char *dev;

  cmd_ip_str = argv[1];
  cmd_via_ip_str = argv[2];
  dev=argv[3];
  uint32_t index = if_nametoindex(dev);
  

  struct in_addr add_v4prefix;
  struct in_addr via_v4prefix;

  inet_pton(AF_INET, cmd_ip_str, &add_v4prefix);
  inet_pton(AF_INET, cmd_via_ip_str, &via_v4prefix);

  ipv4_route_add(add_v4prefix, via_v4prefix, index);

}
