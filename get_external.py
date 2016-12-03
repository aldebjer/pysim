from urllib.request import urlopen
from io	 import BytesIO
import zipfile

#Open the URL to download Eigen 3.3
url = "http://bitbucket.org/eigen/eigen/get/3.3.0.zip"
remotedata = urlopen(url)

#Convert the downloaded data to an in memory file-like object
zipdata = BytesIO()
zipdata.write(remotedata.read())

#Create a zipfile and extract it
myzipfile = zipfile.ZipFile(zipdata)
myzipfile.extractall()