#include "mp4_segment.h"
extern float GetMp4PlayTimeSize(mp4Buffer& inMp4);
int main(int argc, char *argv[])
{
	char *mp4InFile = argv[1];
	char *mp4OutFile = argv[2];
	float startTime = atoi(argv[3]);
	float endTime = atoi(argv[4]);
	
	
	std::string strFile = mp4InFile;
	FILE* pFile = fopen(strFile.c_str(), "rb");
	if (!pFile)
	{
		printf("CMP4File::LoadFile(%s): fopen failed. \n", strFile.c_str());
		return 0;
	}
	fseek(pFile, 0, SEEK_END);
	int iSize = ftell(pFile);
	unsigned char* pData = new unsigned char[iSize];
	fseek(pFile, 0, SEEK_SET);
	if (iSize != fread(pData, 1, iSize, pFile))
	{
		printf("read file failed. \n");
		return 0;
	}
	fclose(pFile);
	mp4Buffer inBuffer;
	inBuffer.m_pBuff = pData;
	inBuffer.m_iSize = iSize;
	mp4Buffer outBuffer;
	
	float Mp4DurationTimeSecond = GetMp4PlayTimeSize(inBuffer);
	int hou = Mp4DurationTimeSecond / 60 / 60;
	int min = (Mp4DurationTimeSecond - hou*60) / 60;
	int sec = (Mp4DurationTimeSecond - hou*60*60 - min*60);
	printf("Mp4DurationTime:%02d:%02d:%02d\n", hou, min, sec);
	
	
	//
	mp4_segment(outBuffer, inBuffer, startTime, endTime);

	{
		std::string strFile = mp4OutFile;
		FILE* pOutFile = fopen(strFile.c_str(), "wb");
		if (!pOutFile)
		{
			printf("CMP4File::LoadFile(%s): fopen failed. \n", strFile.c_str());
			return 0;
		}

		fwrite(outBuffer.m_pBuff, 1, outBuffer.m_iSize, pOutFile);

		fclose(pOutFile);
	}

	delete[]inBuffer.m_pBuff;
	delete[]outBuffer.m_pBuff;
}