# k-flows: In-kernel network flow summary statistics

Run `klog make`, defaulting to `alone` and `silent`, builds a standalone kernel module.

For openvswitch support, run `klog make ovs <ovs dir>`, where `<ovs dir>` points to the ovs directory.

The last argument of `klog make` is always either `verbose` or `silent`. 

Once either module is compiled, load it with `klog start`. 

If compiled with openvswitch, start with
`klog start <ovs dir>/datapath/linux`, i.e. the path to the module.

Unload it with
`klog stop`.

In summary:

a) For a stand-alone version,

```shell
$ sudo klog make
$ sudo klog start
```

b) For a version compatible with openvswitch 1.2.1 (cf. homework at github),
```shell
$ sudo klog make ovs $HOME/homework/openvswitch.git
$ sudo klog start $HOME/homework/openvswitch.git/datapath/linux
```

