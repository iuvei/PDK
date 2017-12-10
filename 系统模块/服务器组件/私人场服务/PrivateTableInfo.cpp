#include "StdAfx.h"
#include "PrivateTableInfo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//构造函数
PrivateTableInfo::PrivateTableInfo()
	:pITableFrame(NULL)
{
	restValue();
}

PrivateTableInfo::~PrivateTableInfo(void)
{
	//关闭定时器
}
void PrivateTableInfo::restAgainValue()
{
	bStart = false;
	bInEnd = false;
	kDismissChairID.clear();
	kNotAgreeChairID.clear();
	fDismissPastTime = 0;
	dwFinishPlayCout = 0;
	dwStartPlayCout = 0;
	fAgainPastTime = 0.0f;
	ZeroMemory(lPlayerWinLose,sizeof(lPlayerWinLose));
	ZeroMemory(lPlayerAction,sizeof(lPlayerAction));
	ZeroMemory(cbLastOfflineReadyState,sizeof(cbLastOfflineReadyState));
	kTotalRecord = tagPrivateRandTotalRecord();

	if (pITableFrame)
	{
		kTotalRecord.kScore.resize(3);
		kTotalRecord.kUserID.resize(3);
		kTotalRecord.kNickName.resize(3);
		kTotalRecord.dwKindID = pITableFrame->GetGameServiceAttrib()->wKindID;
		kTotalRecord.dwVersion = pITableFrame->GetGameServiceAttrib()->dwClientVersion;
		for (int i = 0;i<(int)getChairCout();i++)
		{
			kTotalRecord.kScore[i] = 0;
		}
	}
}
void PrivateTableInfo::restValue()
{
	bStart = false;
	bInEnd = false;
	fOfflinePastTime = 0;
	bPlayCoutIdex = 0;
	bGameTypeIdex = 0;
	bGameRuleIdex = 0;
	cbRoomType = Type_Private;
	dwPlayCout = 0;
	dwRoomNum = 0;
	dwCreaterUserID = 0;
	fDismissPastTime = 0;
	dwFinishPlayCout = 0;
	dwStartPlayCout = 0;
	dwPlayCost = 0;
	fAgainPastTime = 0.0f;
	kHttpChannel = "";

	restAgainValue();
}
void PrivateTableInfo::newRandChild()
{
	tagPrivateRandRecordChild kRecordChild;
	kRecordChild.dwKindID = pITableFrame->GetGameServiceAttrib()->wKindID;
	kRecordChild.dwVersion = pITableFrame->GetGameServiceAttrib()->dwClientVersion;
	kRecordChild.kScore.resize(3);
	kRecordChild.kHandCardCost.resize(3);
	kRecordChild.kNickName.resize(3);
	for (int i = 0;i<3;i++)
	{
		IServerUserItem * pServerItem = pITableFrame->GetTableUserItem(i);
		if (pServerItem == NULL)
		{
			continue;
		}
		kTotalRecord.kNickName[i] = pServerItem->GetNickName();
		kTotalRecord.kUserID[i] = pServerItem->GetUserID();
		kRecordChild.kNickName[i] = kTotalRecord.kNickName[i];
		kRecordChild.kScore[i] = 0;
	}
	GetLocalTime(&kRecordChild.kPlayTime);
	kTotalRecord.kRecordChild.push_back(kRecordChild);
}
WORD PrivateTableInfo::getChairCout()
{
	return pITableFrame->GetChairCount();
}
bool PrivateTableInfo::IsAllOffline()
{
	if (pITableFrame->GetSitUserCount() == 0)
	{
		return false;
	}
	return pITableFrame->GetSitUserCount() == pITableFrame->GetOffLineUserCount();
}
void PrivateTableInfo::setRoomNum(DWORD RoomNum)
{
	kTotalRecord.iRoomNum = (int)RoomNum;
	dwRoomNum = RoomNum;
}
void PrivateTableInfo::writeSocre(tagScoreInfo ScoreInfoArray[], WORD wScoreCount,datastream& daUserDefine)
{

	if (kTotalRecord.kRecordChild.size() == 0)
	{
		ASSERT(false);
		return;
	}
	tagPrivateRandRecordChild& kRecord = kTotalRecord.kRecordChild.back();
	for(WORD i = 0;i < wScoreCount;i++)
	{
		kRecord.kHandCardCost[i] = ScoreInfoArray[i].kHandCardCost;
		kRecord.kScore[i] += (int)ScoreInfoArray[i].lScore;
		kTotalRecord.kScore[i] += (int)ScoreInfoArray[i].lScore;
		lPlayerWinLose[i] += ScoreInfoArray[i].lScore;
	}

	GetLocalTime(&kRecord.kPlayTime);
	kRecord.kRecordGame = daUserDefine;
}