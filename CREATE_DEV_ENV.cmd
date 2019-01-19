@ECHO OFF

ECHO Creating virtual environment

REM Have a look here for details about the below command: https://docs.python.org/3/library/venv.html
py -3.7 -m venv venv

ECHO Activating virtual environment

CALL venv\Scripts\activate.bat

ECHO Installing packages

pip install -r requirements.txt

ECHO Finished

PAUSE