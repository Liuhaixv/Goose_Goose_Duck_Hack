#pragma once

enum OpenProcessState {
    GameNotFound,
    GameFoundAndLoadedDLL,
    GameFoundButFailedToOpen,
    FailedToLoadDLL
};