// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "MyLocalPlayerSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


void UMyGameInstance::AddPlayer(const FString& Pseudo, float NewScore)
{
	PlayerScores = LoadPlayerData();
	for (FPlayerScore& Player : PlayerScores)
	{
		
		if (Player.Pseudo == Pseudo) // Si le pseudo existe déjà
		{
			if (NewScore < Player.Score) // Vérifie si le nouveau score est plus petit
			{
				RemovePlayerDataByName(Pseudo);
				FPlayerScore NewPlayer(Pseudo, NewScore);  
                PlayerScores.Add(NewPlayer);
                SavePlayerData(PlayerScores);
			}
			
			return;
		}
	}

	FPlayerScore NewPlayer(Pseudo, NewScore);  
	PlayerScores.Add(NewPlayer);
	SavePlayerData(PlayerScores);

}

void UMyGameInstance::TriArray(TArray<FPlayerScore>& _PlayerScores)
{
	_PlayerScores.Sort([](const FPlayerScore& A, const FPlayerScore& B)
	{
		return A.Score < B.Score;
	});
}
void UMyGameInstance::SavePlayerData(const TArray<FPlayerScore>& ArrayToSave)
{
	UMyLocalPlayerSaveGame* SaveGameInstance = Cast<UMyLocalPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UMyLocalPlayerSaveGame::StaticClass()));


	// Stocker les données dans l'objet de sauvegarde
	SaveGameInstance->DataArray = ArrayToSave;
	
	 UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("PlayerSaveSlot"), 0);
}

void UMyGameInstance::RemovePlayerDataByName(const FString& PlayerName)
{
	PlayerScores = LoadPlayerData();
	
	for (int32 i = 0; i < PlayerScores.Num(); i++)
	{
		
		if (PlayerScores[i].Pseudo == PlayerName)
		{
			PlayerScores.RemoveAt(i);
			return; // On arrête après la suppression
		}
	}
}



TArray<FPlayerScore> UMyGameInstance::LoadPlayerData()
{
	
	USaveGame* BaseSave = UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0);
	

	UMyLocalPlayerSaveGame* LoadedGame = Cast<UMyLocalPlayerSaveGame>(BaseSave);
	
	// Assigner les données chargées à l'instance actuelle
	
	return PlayerScores = LoadedGame->DataArray;
}

TArray<FPlayerScore> UMyGameInstance::GetClassement()
{
	PlayerScores = LoadPlayerData();
	TriArray(PlayerScores);
	TArray<FPlayerScore> Tmp;
	
	
	if (!PlayerScores.IsEmpty())
	{
		
		for (int32 i = 0; i < 3; i++)
		{
			Tmp.Add(PlayerScores[i]);
			
			UE_LOG(LogTemp, Log, TEXT("✅ Chargement - Nom du joueur : %s, Score : %f"), *Tmp[i].Pseudo, Tmp[i].Score);
		}
		return Tmp;
	}
	return Tmp;
}




