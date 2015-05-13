function StonehengeStart()
Debug("[LUA]running StonehengeStart"); 
--Skybox
sky = ActorDynamicSky();
--Meshes
mesh = new ActorMesh("s1.mesh", "s1", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s2.mesh", "s2", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s3.mesh", "s3", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s4.mesh", "s4", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s5.mesh", "s5", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s6.mesh", "s6", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s7.mesh", "s7", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s8.mesh", "s8", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s9.mesh", "s9", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s10.mesh", "s10", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s11.mesh", "s11", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s12.mesh", "s12", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s13.mesh", "s13", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s14.mesh", "s14", "DeferredDemo/DeferredAthena");
mesh = new ActorMesh("s15.mesh", "s15", "DeferredDemo/DeferredAthena");
--Camera
camera = new CameraFree(true);
end;