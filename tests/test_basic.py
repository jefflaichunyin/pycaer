from pycaer import davis

def on_packet():
    print("get new packet")
    
davis.open()
davis.read()
davis.close()

# def test_version():
#     assert m.__version__ == "0.0.1"


# def test_add():
#     assert m.add(1, 2) == 3


# def test_sub():
#     assert m.subtract(1, 2) == -1
