// Fill out your copyright notice in the Description page of Project Settings.


#include "Management/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Management/MyLocalPlayerSaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


void UMyGameInstance::AddPlayer(const FString& Pseudo, float NewScore)
{
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, "11111" + Pseudo);
	if (!LoadPlayerData().IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, "2222");
		PlayerScores = LoadPlayerData();
	}
	
	for (FPlayerScore& Player : PlayerScores)
	{
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, "3333");
		if (Player.Pseudo == Pseudo) // Si le pseudo existe déjà
		{
			GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, "44444");
			if (NewScore < Player.Score) // Vérifie si le nouveau score est plus petit
			{
				GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, "5555");
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
	if (!LoadPlayerData().IsEmpty())
	{
		PlayerScores = LoadPlayerData();
	}
	
	
	for (int32 i = 0; i < PlayerScores.Num(); i++)
	{
		
		if (PlayerScores[i].Pseudo == PlayerName)
		{
			GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, "Supress");
			PlayerScores.RemoveAt(i);
			SavePlayerData(PlayerScores);
			return; // On arrête après la suppression
		}
	}
}



TArray<FPlayerScore> UMyGameInstance::LoadPlayerData()
{
	USaveGame* BaseSave = UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0);
	if (!BaseSave) 
	{
		UE_LOG(LogTemp, Error, TEXT("LoadGameFromSlot a retourné nullptr ! Aucune sauvegarde trouvée."));
		return TArray<FPlayerScore>(); // Retourne un tableau vide
	}

	UMyLocalPlayerSaveGame* LoadedGame = Cast<UMyLocalPlayerSaveGame>(BaseSave);
	if (!LoadedGame) 
	{
		UE_LOG(LogTemp, Error, TEXT("Le cast de BaseSave en UMyLocalPlayerSaveGame a échoué !"));
		return TArray<FPlayerScore>(); // Retourne un tableau vide
	}

	return LoadedGame->DataArray; 
}


TArray<FPlayerScore> UMyGameInstance::GetClassement()
{
	PlayerScores = LoadPlayerData();
	TriArray(PlayerScores);
	TArray<FPlayerScore> Tmp;

	// Toujours 3 éléments
	for (int32 i = 0; i < 3; i++) 
	{
		if (i < PlayerScores.Num()) 
		{
			// Ajouter un vrai joueur s'il existe
			Tmp.Add(PlayerScores[i]);
		} 
		else 
		{
			// Ajouter une entrée fictive
			FPlayerScore DefaultEntry;
			DefaultEntry.Pseudo = FString::Printf(TEXT("None%d"), i + 1);
			DefaultEntry.Score = TNumericLimits<float>::Max();
			Tmp.Add(DefaultEntry);
		}
	}

	// Logs pour voir le résultat
	for (int32 i = 0; i < Tmp.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("🏆 Classement [%d] - Nom: %s, Score: %f"), 
			i, *Tmp[i].Pseudo, Tmp[i].Score);
	}

	return Tmp;
}







