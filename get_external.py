"""Download the Boost library and unzip it into the external directory.
For use on the windows OS.
"""

from urllib.request import urlopen
from io	 import BytesIO
import zipfile

def read_and_unpack(url):
    """Read and unpack a zip file to the 'external' directory"""

    remotedata = urlopen(url)

    #Convert the downloaded data to an in memory file-like object
    zipdata = BytesIO()
    zipdata.write(remotedata.read())

    #Create a zipfile and extract it
    myzipfile = zipfile.ZipFile(zipdata)
    myzipfile.extractall(path='external')

if __name__ == "__main__":
    read_and_unpack("https://archives.boost.io/release/1.90.0/source/boost_1_90_0.zip")
