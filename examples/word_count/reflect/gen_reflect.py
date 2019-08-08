import os

CLIENT_ACTORS_DIR = "../client_actor"
SERVER_ACTORS_DIR = "../server_actors"
PROTO_DIR = "../protos"
CLIENT_OUTPUT_FILE = "client_reflect.cc"
SERVER_OUTPUT_FILE = "server_reflect.cc"

class_names = []
message_names = []
method_infos = {}

def parse_class(class_file):
  with open(class_file, "r") as f:
    s = f.read()
  s1 = s[s.index("class"):]
  s2 = s1[:s1.index(":")]
  c = s2.replace("\t", "").replace(" ", "")[5:]
  class_names.append(c)
  while s.find("void") != -1:
    s3 = s[s.index("void"):]
    s4 = s3[:s3.index("{")].replace("\t", "").replace(" ", "")
    method = s4[4:s4.index("(")]
    try:
      param_class = s4[s4.index("(const")+6:s4.index("&")]
      method_infos[method] = [c, param_class]
    except:
      pass
    s = s[s.index("void") + 4:]

def parse_message(proto_file):
  with open(proto_file, "r") as f:
    s = f.read()
  while s.find("message") != -1:
    idx = s.index("message")
    s1 = s[idx:]
    s2 = s1[:s1.index("{")]
    message_names.append(s2.replace("\t", "").replace(" ", "")[7:])
    s = s[s.index("message")+7:]

def gen_reflect(actor_dir, output_dir, is_server):
  global class_names, message_names, method_infos
  class_names = []
  message_names = []
  method_infos = {}
  actor_class = "Actor*"

  code = "// Generated code\n\n"
  code += "#include <mtp/reflect.h>\n"
  for d in [actor_dir, PROTO_DIR]:
    for f in os.listdir(d):
      if f.endswith(".h"):
        code += "#include \"%s\"\n" % f
      elif f.endswith(".proto"):
          parse_message("%s/%s" % (d, f))

  for d in [actor_dir]:
    for f in os.listdir(d):
      if f.endswith(".h"):
        parse_class("%s/%s" % (d, f))

  code += "\nnamespace mtp {\n"
  code += "\n%s NewActor(const std::string& actor_name) {\n" % actor_class
  for c in class_names:
    code += "  if (actor_name == \"%s\")\n    return new %s;\n" % (c, c)
  code += "  return NULL;\n}\n"
  code += "\nvoid CallActorMethod(%s actor, const std::string& method, const std::string& message) {\n" % actor_class
  for k, v in method_infos.iteritems():
    code += "  if (method == \"%s\") {\n" % k
    code += "    %s arg;\n" % (v[1], )
    code += "    arg.ParseFromString(message);\n"
    code += "    static_cast<%s*>(actor)->%s(arg);\n    return;\n  }\n" % (v[0], k)
  if not is_server:
    code += "}\n\n}"
  else:
    code += "}\n"
  if is_server:
    code += "\nvoid CallClientService(%s actor, const std::string& method, const Mail& mail) {\n" % actor_class
    code += "  if (method == \"ClientJoin\") {\n"
    code += "    static_cast<ClientService*>(actor)->ClientJoin(mail);\n"
    code += "    return;\n"
    code += "  }\n"
    code += "  if (method == \"ClientLeave\") {\n"
    code += "    static_cast<ClientService*>(actor)->ClientLeave(mail);\n"
    code += "    return;\n"
    code += "  }\n"
    code += "}\n\n}"
  with open(output_dir, "w") as f:
    f.write(code)

if __name__ == "__main__":
  gen_reflect(CLIENT_ACTORS_DIR, CLIENT_OUTPUT_FILE, False)
  gen_reflect(SERVER_ACTORS_DIR, SERVER_OUTPUT_FILE, True)

