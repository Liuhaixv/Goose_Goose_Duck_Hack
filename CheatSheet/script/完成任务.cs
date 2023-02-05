using UnityEngine;
using Handlers.LobbyHandlers;
using Handlers.GameHandlers.TaskHandlers;
using Objects;
using Il2CppSystem.Collections.Generic;

public class CompleteTask : MonoBehaviour {
    private void Start() {
        try{
            //获取当前游戏场景中的Handlers
            GameObject handlers = GameObject.Find("Handlers");

            //获取LobbySceneHandler
            LobbySceneHandler lobbySceneHandler = handlers.GetComponent<LobbySceneHandler>();
            //获取TasksHandler
            TasksHandler tasksHandler = handlers.GetComponent<TasksHandler>();

            UnityExplorer.ExplorerCore.Log(tasksHandler.assignedTasks);
            //Generic.Dictionary<string, GameTask> assignedTasks = tasksHandler.assignedTasks;
			
            // foreach(var item in assignedTasks)
            // {
            //     //Debug.Log(item);
            // }
			
           //tasksHandler.CompleteTask(taskName, true,false,false,true);
        }
        catch( Exception e ){
            UnityExplorer.ExplorerCore.Log("err");
        }
    }
}