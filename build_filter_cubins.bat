@ECHO OFF
SETLOCAL ENABLEEXTENSIONS

ECHO we have: %*

CALL cudaenv.bat %*

IF [%shadermodelnum%]==[] (
	FOR %%s IN (sm_20, sm_21) DO (
		ECHO Compiling for %%s
		CALL cudacompile.bat filter %%s
	)
	FOR %%s IN (sm_30, sm_35, sm_50, sm_52, sm_60, sm_61, sm_70) DO (
		ECHO Compiling for %%s
		CALL cudacompile.bat filter %%s
	)
) ELSE (
	ECHO Compiling for only sm_%shadermodelnum%
	CALL cudacompile.bat filter sm_%shadermodelnum%
)
