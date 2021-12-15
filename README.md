# jesSubmit

Clone to z/os uss:

  git clone --recursive https://github.com/joepun76/jesSubmit.git
  
Build:

  cd build
  jesBuild.sh
  
Test:

  testJM01 'your jcl pds name' 'member'

Sample result:

  testJM01 'XXX.YYY.JCL' 'ASMSSI80'

testJM01 'JOE.TOOL.JCL' 'ASMSSI80'

dsn (JOE.TOOL.JCL                                ), member(ASMSSI80)
daRc ... (0)
ddname (SYS00001)
INTRDR alloc rc ... (0)
INTRDR ddname (SYS00002)
jobId (JOB01190)
