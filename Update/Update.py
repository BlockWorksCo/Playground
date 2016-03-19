




import re
import datetime


ts 	= '%s'%datetime.datetime.now()
t 	= open('find-ip-address').read()
ip 	= re.compile('Your.*\>([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)').findall(t)[0]


template = """
<html>
<head>
</head>
<body>
<p>%s</p>
<p>%s</p>
</body>
</html>
"""

open('Home.html','w').write(template%(ip,ts))




