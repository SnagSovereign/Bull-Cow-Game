// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    //Get the path of WordList.txt
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/WordList.txt");
    //Set the Words Array to the words in WordList.txt
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    //Filter the Words Array to only contain valid words
    Words = GetValidWords(Words);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Congratulations, you win!"));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len()) // If the guess is NOT the correct length
    {
        PrintLine(TEXT("The Hidden Word is %i characters long, try again..."), HiddenWord.Len());
        return;
    }

    if (!IsIsogram(Guess)) // If the guess is NOT an isogram
    {
        // tell the user that the word must be an isogram
        PrintLine(TEXT("No repeating letters!"));
        PrintLine(TEXT("Guess again..."));
        return;
    }

    --Lives; // remove a life
    PrintLine(TEXT("Lost a life!"));

    if (Lives <= 0)
    {
        // Print fail message and HiddenWord
        PrintLine(TEXT("You lost :(  The word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    // Show the player the Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Guess again. You now have %i lives."), Lives);
}

void UBullCowCartridge::SetupGame()
{
    bGameOver = false;
    HiddenWord = Words[FMath::RandRange(0, Words.Num() - 1)];
    Lives = HiddenWord.Len() * 2;
  
    // welcome player
    PrintLine(TEXT("Hello there! Welcome to Bulls and Cows!"));
    PrintLine(TEXT("Guess the %i letter word."), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives!"), Lives);
    PrintLine(TEXT("Type your guess and press enter..."));

    //PrintLine(TEXT("The HiddenWord is: %s"), *HiddenWord);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again..."));
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <=8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}