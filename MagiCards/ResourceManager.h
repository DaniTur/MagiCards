#pragma once
#include <Windows.h>
#include "resource.h" // not needed here, but saves us from having to include it evey time
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class ResourceManager
{
public:
    static SDL_Surface* LoadImageFromResource(HINSTANCE hInstance, LPCWSTR lpName) {

        HRSRC hResInfo = FindResourceW(hInstance, lpName, RT_RCDATA);
        if (hResInfo == NULL) {
            std::cout << "Error finding the resource on the memory" << std::endl;
            return nullptr; // Error finding the resource on the memory 
        }

        HGLOBAL hResData = LoadResource(hInstance, hResInfo);
        if (hResData == NULL) {
            std::cout << "Error loading the resource" << std::endl;
            return nullptr; // Error loading the resource
        }

        DWORD dwSize = SizeofResource(hInstance, hResInfo);
        if (dwSize == 0) {
            std::cout << "Error obtaining the resource size" << std::endl;
            return nullptr; // Error obtaining the resource size
        }

        void* pData = LockResource(hResData);
        if (pData == NULL) {
            std::cout << "Error blocking the resource" << std::endl;
            return nullptr; // Error blocking the resource
        }

        // Create an interface object for SDL to access resources from memory instead of accessing a resource from disk
        SDL_RWops* rwops = SDL_RWFromConstMem(pData, dwSize);
        if (rwops == NULL) {
            std::cout << "Error creating SDL_RWops object" << std::endl;
            return nullptr; // Error creating SDL_RWops object
        }

        // Load the image from SDL_RWops as surface
        SDL_Surface* surface = IMG_Load_RW(rwops, 1); // 1 to free SDL_RWops automaticaly
        if (surface == nullptr) {
            std::cout << "Error loading image as surface" << std::endl;
            return nullptr; // Error loading image as surface
        }

        // Free the resource of the memory
        FreeResource(hResData);

        return surface;
    }
    // Add methods to load different type of resources if needed
};