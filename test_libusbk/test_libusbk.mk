##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=test_libusbk
ConfigurationName      :=Debug
WorkspacePath          := "D:\tmp\USB"
ProjectPath            := "D:\tmp\USB\test_libusbk"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jirawat
Date                   :=01/19/15
CodeLitePath           :="C:\Program Files (x86)\CodeLite"
LinkerName             :=D:/bin/TDM-GCC-64/bin/g++.exe 
SharedObjectLinkerName :=D:/bin/TDM-GCC-64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)EXAMPLES_USE_BENCHMARK_CONFIGURE 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="test_libusbk.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=D:/bin/TDM-GCC-64/bin/windres.exe 
LinkOptions            :=  -lkernel32 -luser32
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)D:/lib/libusbK-dev-kit/includes 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)usbK 
ArLibs                 :=  "usbK" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)D:/lib/libusbK-dev-kit/bin/dll/amd64 $(LibraryPathSwitch)D:/lib/libusbK-dev-kit/bin/lib/amd64 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := D:/bin/TDM-GCC-64/bin/ar.exe rcu
CXX      := D:/bin/TDM-GCC-64/bin/g++.exe 
CC       := D:/bin/TDM-GCC-64/bin/gcc.exe 
CXXFLAGS :=  -g -O0 -Wall -mwin32 $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -mwin32 $(Preprocessors)
ASFLAGS  := 
AS       := D:/bin/TDM-GCC-64/bin/as.exe 


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
UNIT_TEST_PP_SRC_DIR:=D:\bin\UnitTest++-1.3
Objects0=$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IntermediateDirectory)/examples.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c $(IntermediateDirectory)/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/tmp/USB/test_libusbk/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM "main.c"

$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) "main.c"

$(IntermediateDirectory)/examples.c$(ObjectSuffix): examples.c $(IntermediateDirectory)/examples.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/tmp/USB/test_libusbk/examples.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/examples.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/examples.c$(DependSuffix): examples.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/examples.c$(ObjectSuffix) -MF$(IntermediateDirectory)/examples.c$(DependSuffix) -MM "examples.c"

$(IntermediateDirectory)/examples.c$(PreprocessSuffix): examples.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/examples.c$(PreprocessSuffix) "examples.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) ./Debug/*$(ObjectSuffix)
	$(RM) ./Debug/*$(DependSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile).exe
	$(RM) "../.build-debug/test_libusbk"


