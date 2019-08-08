import os

if __name__ == "__main__":
  for f in os.listdir("."):
    if f.endswith(".proto"):
      os.system("protoc --cpp_out=./ %s" % f)

