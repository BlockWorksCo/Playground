




import re
import datetime


ts 	= '%s'%datetime.datetime.now()
t 	= open('find-ip-address').read()
ip 	= re.compile('Your.*\>([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)').findall(t)[0]

log 	= open('RecentSpeedTestHistory').read()
log 	= log.replace('\n','<br>')

template = """
<html>
<head>
</head>
<body>
<p>%s</p>
<p>%s</p>
<br>
<p><code>%s</code></p>
</body>
</html>
"""

open('Home.html','w').write(template%(ip,ts,log))




