"""Download the Boost and Eigen library and unzip it into the current 
directory.
"""

from urllib.request import urlopen
from io	 import BytesIO
import zipfile

def read_and_unpack(url):
    """Read and unpack a zip file to the current directory"""

    remotedata = urlopen(url)

    #Convert the downloaded data to an in memory file-like object
    zipdata = BytesIO()
    zipdata.write(remotedata.read())

    #Create a zipfile and extract it
    myzipfile = zipfile.ZipFile(zipdata)
    myzipfile.extractall()

if __name__ == "__main__":
    #read_and_unpack("https://sourceforge.net/projects/boost/files/boost/1.77.0/boost_1_77_0.zip/download")
    read_and_unpack("https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.3.7.zip")
