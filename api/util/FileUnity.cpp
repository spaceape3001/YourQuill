#include "AbstractFile.hpp"
#include "FileName.hpp"
#include "FileUtils.hpp"
#include "LogFile.hpp"
#include "Logging.hpp"
#include "String.hpp"
#include "ThreadId.hpp"
#include "Vector.hpp"
#include "XmlFile.hpp"
#include "XmlUtils.hpp"

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"


#if 0
//#include "meta/Variant.hpp"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#endif

#include <fstream>

#include <QByteArray>
#include <QFileInfo>
#include <QString>
#include <QTimer>

#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>


#include "AbstractFile.ipp"
#include "FileName.ipp"
#include "FileUtils.ipp"
#include "LogFile.ipp"
#include "XmlFile.ipp"
#include "XmlUtils.ipp"
