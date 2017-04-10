import sys, json, struct, os
from subprocess import Popen, PIPE

base_directory = "C:/work/base_practice/cpp_oop/.build"

def get_message():
    raw_length = sys.stdin.read(4)
    if len(raw_length) == 0:
        sys.exit(0)
    length = struct.unpack('@I', raw_length)[0]
    message = sys.stdin.read(length)
    return json.loads(message)

def encode_message(content):
    content = json.dumps(content)
    length = struct.pack('@I', len(content))
    return {'length': length, 'content': content}

def send_message(message):
    sys.stdout.write(message['length'])
    sys.stdout.write(message['content'])
    sys.stdout.flush()

environment = os.environ.copy()
environment["PATH"] = base_directory + ";" + environment["PATH"]
p = Popen(base_directory + '/nmh.exe', stdin = PIPE, env = environment)
content = "{ \"command\": \"process\", \"args\" : [] }"
size = struct.pack('@I', len(content))
p.stdin.write(size)
p.stdin.write(content)
p.stdin.flush()
p.stdin.close()
p.wait()