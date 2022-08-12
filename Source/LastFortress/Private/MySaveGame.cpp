// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"

TArray<FSaveData> UMySaveGame::GetSortSaveDatas()
{

	TArray<FSaveData> SaveDataArray;

	for (auto temp : SaveDatas)
	{
		SaveDataArray.Add(temp.Value);
	}

	SaveDataArray.Sort([](const FSaveData& A, const FSaveData& B)
   {
			return A.PlayerScore > B.PlayerScore;
	});

	return SaveDataArray;
}
