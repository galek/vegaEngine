/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	Script: 		OGRE MESH.xml Importer
	Description:	Builds a 3D Mesh from the data included in the selected MESH.xml generated from 
					the "OgreXMLConverter.exe". Includes UVW Co-ordinates.
	
	Created By:	Travis Williams
		
	[Version 1.0 | 28-FEB-2013 | Travis]
		- Script creation

*/ ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

(

	local gui_title					= "OGRE MESH Importer - v1.0"
	local gui_version				= "by Travis Williams @ all3dmax.com"
	local gui_width					= 350
	local gui_height				= 125
	local gui_main					= undefined
	local gui_floater				= undefined
	
	local my_fopen					= undefined
	local my_fileXML				= undefined
	local my_fileLine				= undefined
	local my_filterLine				= undefined
	local my_checkLine			= undefined
	local my_tempArray			= #()
	
-- XML data
	local my_FaceArray			= #()
	local my_PositionArray		= #()
	local my_NormalArray		= #()
	local my_TextureArray		= #()
	local my_MeshName			= undefined
	
	local my_mesh					= undefined
	
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- function: read XML
	
	function _readXML = (
	
		my_FaceArray			= #()
		my_PositionArray		= #()
		my_NormalArray		= #()
		my_TextureArray		= #()
		my_MeshName			= undefined
		
	-- read XML
		while not eof my_fopen do (
			
		-- get current line
			my_fileLine = readLine my_fopen
			
		-- find V1 - V2 - V3 -------------------------------------------------------------------------------------------------------------------------------------------------
			
			if ((matchPattern my_fileLine pattern:"*v1*" ignoreCase:true) == true) then (
			
				my_filterLine		= (filterstring my_fileLine "\"")
				my_posArray		= #()
			
				for A1 = 1 to my_filterLine.count do (
					if ((my_filterLine[A1] as integer) != undefined) then (append my_posArray my_filterLine[A1])
				)
				
				if (my_posArray.count == 3) then (
					append my_FaceArray (point3 ((my_posArray[1] as float)+1) ((my_posArray[2] as float)+1) ((my_posArray[3] as float)+1))
				)
				
			)
			
		-- find Position -----------------------------------------------------------------------------------------------------------------------------------------------------
			
			if ((matchPattern my_fileLine pattern:"*<position*" ignoreCase:true) == true) then (
			
				my_filterLine		= (filterstring my_fileLine "\"")
				my_posArray		= #()
			
				for A2 = 1 to my_filterLine.count do (
					if ((my_filterLine[A2] as integer) != undefined) then (append my_posArray my_filterLine[A2])
				)
				
				if (my_posArray.count == 3) then (
					append my_PositionArray (point3 (my_posArray[1] as float) (my_posArray[2] as float) (my_posArray[3] as float))
				)			
			
			)
			
		-- find Normal -------------------------------------------------------------------------------------------------------------------------------------------------------
			
			if ((matchPattern my_fileLine pattern:"*<normal*" ignoreCase:true) == true) then (
			
				my_filterLine		= (filterstring my_fileLine "\"")
				my_posArray		= #()
			
				for A3 = 1 to my_filterLine.count do (
					if ((my_filterLine[A3] as integer) != undefined) then (append my_posArray my_filterLine[A3])
				)
				
				if (my_posArray.count == 3) then (
					append my_NormalArray (point3 (my_posArray[1] as float) (my_posArray[2] as float) (my_posArray[3] as float))
				)			
			
			)			
			
		-- find Texture ------------------------------------------------------------------------------------------------------------------------------------------------------
			
			if ((matchPattern my_fileLine pattern:"*<texcoord*" ignoreCase:true) == true) then (
			
				my_filterLine		= (filterstring my_fileLine "\"")
				my_posArray		= #()
			
				for A4 = 1 to my_filterLine.count do (
					if ((my_filterLine[A4] as integer) != undefined) then (append my_posArray my_filterLine[A4])
				)
				
				if (my_posArray.count == 2) then (
					append my_TextureArray (point3 (my_posArray[1] as float) (1-(my_posArray[2] as float)) (0.0))
				)			
			
			)	
			
		-- find Mesh Name ----------------------------------------------------------------------------------------------------------------------------------------------------
			
-- 			if ((matchPattern my_fileLine pattern:"*name=*" ignoreCase:true) == true) then (
-- 			
-- 				my_filterLine		= (filterstring my_fileLine "\"")			
-- 				my_MeshName = my_filterLine[2]
-- 			
-- 			)		
			
		) -- end while
	) -- end function
	
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- function: build mesh
	
	function _buildMESH = (
		
		my_mesh = undefined

		my_mesh = mesh vertices:my_PositionArray faces:my_FaceArray tverts:my_TextureArray
		my_mesh.name = my_MeshName
		
		update my_mesh
		buildtvfaces my_mesh false
		
		for A1 = 1 to my_FaceArray.count do (
			settvface my_mesh A1 my_FaceArray[A1]
		)
		
		update my_mesh
		
	-- rotate mesh back to correct orientation
		rotate my_mesh (angleaxis 90 [1,0,0])
		
	-- convert to Editable Poly
		convertToPoly my_mesh
		
		select my_mesh
		max modify mode
		
	-- remove floating vertices
		subObjectLevel = 1
			max select all
			my_mesh.weldThreshold = 0.001
			my_mesh.weldFlaggedVertices ()
			my_mesh.deleteIsoVerts ()
			my_mesh.DeleteIsoMapVerts ()
			max select none
		
		subObjectLevel = 4
			max select all
			my_mesh.buttonOp #ClearSmoothingGroups
			max select none
			
		subObjectLevel = 0
				
	-- collapse stack
		CollapseStack my_mesh
		
	) -- end function
	
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- create GUI

	rollout gui_main gui_version (

		label label_name "Enter a name for the Imported MESH" pos:[10, 5] width:(gui_width-25) height:16
		edittext edit_name "" text:"ImportedMESH" pos:[5, 25] width:(gui_width-25) height:16
		button button_importMesh "Import MESH.xml" pos:[5, 50] width:(gui_width-25) height:45 enabled:true

	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	-- Button: Import OGRE Mesh
		
		on button_importMesh pressed do (
			
		-- select XML file
			my_fileXML = getOpenFileName types:"Ogre Mesh (*.MESH.xml)|*.MESH.xml|All (*.*)|*.*|"
			
		-- check if XML file was selected
			if (my_fileXML == undefined) then return()
			
		-- open file
			my_fopen = openFile my_fileXML
			
		-- read XML
			_readXML()
			
		-- close file
			close my_fopen
			
		-- check if data was obtained from XML
			if (my_PositionArray.count > 1) and (my_FaceArray.count > 1) then (
				
			-- read mesh NAME
				my_MeshName = edit_name.text
				
			-- build MESH
				_buildMESH()
			
			-- update viewport
				completeRedraw()
				
			-- show finished message in listener
				print "Import Complete..."
				
			)
			else (
				messageBox "There was a problem reading the XML file." title:"Failed..."
				return()
			)
			
		) -- end button
		
	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		
	) -- end gui
	
	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	gui_floater = newRolloutFloater gui_title gui_width gui_height
	addRollout gui_main gui_floater
	
	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

)