#!/usr/bin/env python 

import sys
import optparse
import socket
import select
import errno
import pytun

class TunnelServer(object):

    def __init__(self, taddr, tdstaddr, tmask, tmtu):
        self._tun = pytun.TunTapDevice(flags=pytun.IFF_TUN|pytun.IFF_NO_PI)
        self._tun.addr = taddr
        self._tun.dstaddr = tdstaddr
        self._tun.netmask = tmask
        self._tun.mtu = tmtu
        self._tun.up()

    def run(self):
        mtu = self._tun.mtu
        r = [self._tun]; w = []; x = []
        to_tun = ''
        while True:

            try:
                r, w, x = select.select(r, w, x)
                if self._tun in r:
                    to_sock = self._tun.read(mtu)
                    print('r%d'%(len(to_sock)))
                if self._tun in w:
                    print('w%d'%(len(to_tun)))
                    self._tun.write(to_tun)
                    to_tun = ''

                r = []; w = []
                if to_tun:
                    w.append(self._tun)
                else:
                    r.append(self._tun)

            except (select.error, socket.error, pytun.Error), e:
                if e[0] == errno.EINTR:
                    continue
                print >> sys.stderr, str(e)
                break

def main():
    parser = optparse.OptionParser()
    parser.add_option('--tun-addr', dest='taddr',
            help='set tunnel local address')
    parser.add_option('--tun-dstaddr', dest='tdstaddr',
            help='set tunnel destination address')
    parser.add_option('--tun-netmask', default='255.255.255.0',dest='tmask',
            help='set tunnel netmask')
    parser.add_option('--tun-mtu', type='int', default=1500,dest='tmtu',
            help='set tunnel MTU')
    opt, args = parser.parse_args()
    if not (opt.taddr and opt.tdstaddr):
        parser.print_help()
        return 1
    try:
        server = TunnelServer(opt.taddr, opt.tdstaddr, opt.tmask, opt.tmtu)
    except (pytun.Error, socket.error), e:
        print >> sys.stderr, str(e)
        return 1
    server.run()
    return 0

if __name__ == '__main__':
    sys.exit(main())



