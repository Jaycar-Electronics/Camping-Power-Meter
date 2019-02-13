#python3

from pathlib import Path
import re

infile = './webcode/index.html'
outfile = './test/html.h'
infile = input('input file? default: {}'.format(infile)) or infile
outfile = input('output file? default: {}'.format(outfile)) or outfile
print('converting..')

contents = Path(infile).open().read()
mark_comment = False
out = '''
#ifndef _HTML_H__
#define _HTML_H__
const char indexhtml[] PROGMEM = "'''
for line in contents.split('\n'):
    line = line.strip()
    if '/*' in line:
        mark_comment = True
    elif '*/' in line:
        mark_comment = False
        continue

    if mark_comment:
        continue #skip comment

    if line == '':
        continue; #skip
    if '"' in line and "'" in line:
        print('WARNING!, a line of code contains both single and double quotes; change this if you can!')

    #remove spaces around operators
    line = re.sub(r' ?([;/=<>+*(),]) ?',r'\1',line)
	#add space around var
	line = re.sub(r'var',' var',line)
    print(line)
    #strip line comments and use single quotes
    line = re.sub(r'//(.*)$',r'',line)
    line = re.sub(r'"',r"'",line)
   
    out += line

out += '''"; 
#endif
'''
Path(outfile).open('w').write(out)

print(out)
