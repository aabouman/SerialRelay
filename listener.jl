using ZMQ

# %%
s1 = Socket(ZMQ.SUB)
ZMQ.subscribe(s1)
ZMQ.connect(s1, "tcp://127.0.0.1:5556")

# %%
msg = ZMQ.recv(s1)
