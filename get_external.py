"""Download the Eigen vector library and unzip it into the current 
directory.
"""

from urllib.request import urlopen
from io	 import BytesIO
import zipfile

def read_and_unpack():
    """Reand and unpack Eigen to the current directory"""
    #Open the URL to download Eigen 3.3
    url = "http://bitbucket.org/eigen/eigen/get/3.3.7.zip"
    remotedata = urlopen(url)

    #Convert the downloaded data to an in memory file-like object
    zipdata = BytesIO()
    zipdata.write(remotedata.read())

    #Create a zipfile and extract it
    myzipfile = zipfile.ZipFile(zipdata)
    myzipfile.extractall()

if __name__ == "__main__":
    read_and_unpack()
