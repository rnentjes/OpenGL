##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=OpenGL
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/data/Development/OpenGL"
ProjectPath            := "/home/data/Development/OpenGL"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Rien Nentjes
Date                   :=18-01-12
CodeLitePath           :="/home/rnentjes/.codelite"
LinkerName             :=gcc
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=gcc
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
MakeDirCommand         :=mkdir -p
CmpOptions             := -g $(Preprocessors)
LinkOptions            :=  
IncludePath            :=  "$(IncludeSwitch)." "$(IncludeSwitch)." 
RcIncludePath          :=
Libs                   :=$(LibrarySwitch)GLEW $(LibrarySwitch)GL $(LibrarySwitch)glut 
LibPath                := "$(LibraryPathSwitch)." 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/triangle$(ObjectSuffix) $(IntermediateDirectory)/glass$(ObjectSuffix) $(IntermediateDirectory)/shaderUtils$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@$(MakeDirCommand) $(@D)
	$(LinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

makeDirStep:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/triangle$(ObjectSuffix): triangle.c $(IntermediateDirectory)/triangle$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/data/Development/OpenGL/triangle.c" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/triangle$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/triangle$(DependSuffix): triangle.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/triangle$(ObjectSuffix) -MF$(IntermediateDirectory)/triangle$(DependSuffix) -MM "/home/data/Development/OpenGL/triangle.c"

$(IntermediateDirectory)/triangle$(PreprocessSuffix): triangle.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/triangle$(PreprocessSuffix) "/home/data/Development/OpenGL/triangle.c"

$(IntermediateDirectory)/glass$(ObjectSuffix): glass.c $(IntermediateDirectory)/glass$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/data/Development/OpenGL/glass.c" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/glass$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/glass$(DependSuffix): glass.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/glass$(ObjectSuffix) -MF$(IntermediateDirectory)/glass$(DependSuffix) -MM "/home/data/Development/OpenGL/glass.c"

$(IntermediateDirectory)/glass$(PreprocessSuffix): glass.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/glass$(PreprocessSuffix) "/home/data/Development/OpenGL/glass.c"

$(IntermediateDirectory)/shaderUtils$(ObjectSuffix): shaderUtils.c $(IntermediateDirectory)/shaderUtils$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/data/Development/OpenGL/shaderUtils.c" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/shaderUtils$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/shaderUtils$(DependSuffix): shaderUtils.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/shaderUtils$(ObjectSuffix) -MF$(IntermediateDirectory)/shaderUtils$(DependSuffix) -MM "/home/data/Development/OpenGL/shaderUtils.c"

$(IntermediateDirectory)/shaderUtils$(PreprocessSuffix): shaderUtils.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/shaderUtils$(PreprocessSuffix) "/home/data/Development/OpenGL/shaderUtils.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/triangle$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/triangle$(DependSuffix)
	$(RM) $(IntermediateDirectory)/triangle$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/glass$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/glass$(DependSuffix)
	$(RM) $(IntermediateDirectory)/glass$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/shaderUtils$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/shaderUtils$(DependSuffix)
	$(RM) $(IntermediateDirectory)/shaderUtils$(PreprocessSuffix)
	$(RM) $(OutputFile)


