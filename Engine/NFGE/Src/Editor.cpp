#include "Precompiled.h"
#include "Editor.h"

#include "World.h"
#include "GameObject.h"
#include "Component.h"
#include "MeshRenderComponent.h"
#include "Service.h"
#include "CameraService.h"
#include "NFGE.h"

using namespace NFGE;

namespace
{
	void ShowExampleMenuFile()
	{
		ImGui::MenuItem("(dummy menu)", NULL, false, false);
		if (ImGui::MenuItem("New")) {}
		if (ImGui::MenuItem("Open", "Ctrl+O")) {}
		if (ImGui::BeginMenu("Open Recent"))
		{
			ImGui::MenuItem("fish_hat.c");
			ImGui::MenuItem("fish_hat.inl");
			ImGui::MenuItem("fish_hat.h");
			if (ImGui::BeginMenu("More.."))
			{
				ImGui::MenuItem("Hello");
				ImGui::MenuItem("Sailor");
				if (ImGui::BeginMenu("Recurse.."))
				{
					ShowExampleMenuFile();
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save", "Ctrl+S")) {}
		if (ImGui::MenuItem("Save As..")) {}
		ImGui::Separator();
		if (ImGui::BeginMenu("Options"))
		{
			static bool enabled = true;
			ImGui::MenuItem("Enabled", "", &enabled);
			ImGui::BeginChild("child", ImVec2(0, 60), true);
			for (int i = 0; i < 10; i++)
				ImGui::Text("Scrolling Text %d", i);
			ImGui::EndChild();
			static float f = 0.5f;
			static int n = 0;
			static bool b = true;
			ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
			ImGui::InputFloat("Input", &f, 0.1f);
			ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
			ImGui::Checkbox("Check", &b);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Colors"))
		{
			float sz = ImGui::GetTextLineHeight();
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
				ImVec2 p = ImGui::GetCursorScreenPos();
				ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
				ImGui::Dummy(ImVec2(sz, sz));
				ImGui::SameLine();
				ImGui::MenuItem(name);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Disabled", false)) // Disabled
		{
			IM_ASSERT(0);
		}
		if (ImGui::MenuItem("Checked", NULL, true)) {}
		if (ImGui::MenuItem("Quit", "Alt+F4")) {}
	}

	void ShowMetaClassInInspector(const NFGE::Core::Meta::MetaClass* metaClassPtr, uint8_t* rawPtr)
	{
		if (ImGui::CollapsingHeader(metaClassPtr->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (size_t i = 0; i < metaClassPtr->GetFieldCount(); ++i)
			{
				auto metaField = metaClassPtr->GetField(i);
				if (metaField->GetMetaType() == Core::Meta::DeduceType<Math::Vector3>())
				{
					Math::Vector3* data = (Math::Vector3*)(rawPtr + metaField->GetOffset());
					ImGui::DragFloat3(metaField->GetName(), &(data->x));
				}
				else if (metaField->GetMetaType() == Core::Meta::DeduceType<Math::Vector4>())
				{
					Math::Vector4* data = (Math::Vector4*)(rawPtr + metaField->GetOffset());
					ImGui::ColorPicker4(metaField->GetName(), &(data->x));
				}
				else if (metaField->GetMetaType() == Core::Meta::DeduceType<Math::EditorQuaternion>())
				{
					Math::EditorQuaternion* data = (Math::EditorQuaternion*)(rawPtr + metaField->GetOffset());

					Math::Vector3& eurlaAngle = data->mEularAngle;
					eurlaAngle *= NFGE::Math::Constants::RadToDeg;

					ImGui::DragFloat3(metaField->GetName(), &eurlaAngle.x);

					eurlaAngle *= NFGE::Math::Constants::DegToRad;

					data->mQuaternion = Math::Quaternion::ToQuaternion(eurlaAngle.x, eurlaAngle.y, eurlaAngle.z);
				}
				else if (metaField->GetMetaType() == Core::Meta::DeduceType<std::string>())
				{
					std::string* data = (std::string*)(rawPtr + metaField->GetOffset());

					char temp[256];
					strcpy(temp, data->c_str());
					ImGui::InputText(metaField->GetName(), temp, 256);
					
					*data = temp;
				}
				else if (metaField->GetMetaType() == Core::Meta::DeduceType<float>())
				{
					float* data = (float*)(rawPtr + metaField->GetOffset());
					ImGui::DragFloat(metaField->GetName(), data, 0.1f, 0.0f, 360.0f);
				}
				else if (metaField->GetMetaType() == Core::Meta::DeduceType<bool>())
				{
					bool* data = (bool*)(rawPtr + metaField->GetOffset());
					ImGui::Checkbox(metaField->GetName(), data);
				}
				else if (metaField->GetMetaType() == Core::Meta::DeduceType<NFGE::Graphics::MeshBuffer::Topology>())
				{
					NFGE::Graphics::MeshBuffer::Topology* data = (NFGE::Graphics::MeshBuffer::Topology*)(rawPtr + metaField->GetOffset());

					int selectedItem = static_cast<int>(*data);
					const char* items[]{ "Points", "Lines", "Triangles" };

					ImGui::Combo("Topology", &selectedItem, items, IM_ARRAYSIZE(items));

					*data = static_cast<NFGE::Graphics::MeshBuffer::Topology>(selectedItem);
				}
			}
		}
	}
}
Editor::Editor(World& world)
	:mWorld(world)
{

}
void NFGE::Editor::ShowMainWindowWithDockSpace()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Main", nullptr, window_flags);
	ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	ImGui::End();

	ImGui::PopStyleVar(3);
}

void Editor::ShowWorldView()
{
	ImGui::Begin("World");
	if (ImGui::TreeNode("Service"))
	{
		for (auto& service : mWorld.mServices)
		{
			if (ImGui::Selectable(service->GetMetaClass()->GetName(), service.get() == mSelectedService))
			{
				mSelectedService = service.get();
				mSelectedGameObject = nullptr;
			}
			if (service.get() == mSelectedService)
			{
				mSelectedService->WorldViewUI();
			}
		}

		ImGui::TreePop();
	}
	
	if (ImGui::TreeNode("GameObjects"))
	{
		bool isDraging = false;
		bool isDroped = false;
		std::string moveTo = "";

		for (auto gameObject : mWorld.mUpdateList)
		{
			if (gameObject->mParent == nullptr)
			{
				ShowGameObjectInWorldView(gameObject, moveTo, isDraging, isDroped);
			}
		}

		if (isDraging)
		{
			ImGui::Selectable("\t|");
			ImGui::Selectable("\t|");
			ImGui::Selectable("\tV");
			ImGui::Selectable("~root");

			if (!isDroped)
			{
				if (ImGui::BeginDragDropTarget())
				{
					ImGuiDragDropFlags target_flags = 0;
					target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
					//target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
					//target_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Choosing PayLoad", target_flags))
					{
						auto movingFrom = *(std::string*)payload->Data;
						auto movingGameObj = mWorld.Find(movingFrom);
						
						if (movingGameObj->mParent)
						{
							movingGameObj->mParent->RemoveChild(movingGameObj.Get());
							movingGameObj->ResetParent(nullptr);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
		}

		
		



		if (mSelectedGameObject != nullptr)
			mSelectedService = nullptr;

		ImGui::TreePop();
	}
	ImGui::End();
}
void Editor::ShowInspectorView()
{
	ImGui::Begin("Inspector");
	if (mSelectedService)
	{
		mSelectedService->InspectorUI(ShowMetaClassInInspector);
	}
	else if (mSelectedGameObject)
	{
		mSelectedGameObject->InspectorUI(ShowMetaClassInInspector);
	}
	ImGui::End();
}

void NFGE::Editor::ShowMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			//ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			//if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			//if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			//ImGui::Separator();
			//if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			//if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			//if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			
			if (ImGui::BeginMenu("Add GameObject"))
			{
				if(ImGui::MenuItem("Empty GameObject"))
				{
					mWorld.CreateEmpty("Empty");
				}
				if (ImGui::BeginMenu("Geometry"))
				{
					for (size_t i = 0; i < (size_t)MeshRenderGeometryType::MAX; i++)
					{
						if (ImGui::MenuItem(NFGE::MeshRenderGeometryTypeName[i]))
						{
							auto gameObject = mWorld.CreateEmpty(NFGE::MeshRenderGeometryTypeName[i]);
							auto meshRenderComponent = gameObject->AddComponent<NFGE::MeshRenderComponent>();
							meshRenderComponent->mGemotryType = i;
							meshRenderComponent->Initialize();
						}
					}
					
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void NFGE::Editor::ShowSceneView()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Scene");
	
	ImGui::PopStyleVar(1);

	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();

	vMin.x += ImGui::GetWindowPos().x;
	vMin.y += ImGui::GetWindowPos().y;
	vMax.x += ImGui::GetWindowPos().x;
	vMax.y += ImGui::GetWindowPos().y;

	// Show border for debugging
	ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255));

	auto width = NFGE::Graphics::GraphicsSystem::Get()->GetBackBufferWidth();
	auto height = NFGE::Graphics::GraphicsSystem::Get()->GetBackBufferHeight();
	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

	auto lastRenderTarget = NFGE::Graphics::PostProcessManager::Get()->GetLastRenderTargetPtr();
	ImGui::Image(lastRenderTarget->GetShaderResourceView(), { (vMax.y - vMin.y) * aspectRatio, vMax.y - vMin.y });

	if (ImGui::IsItemHovered())
	{
		NFGE::sApp.SoSoCameraControl(10.0f, 1.0f, mWorld.GetService<CameraService>()->GetActiveCameraEntry(), NFGE::sApp.GetDeltaTime());
	}
	

	ImGui::CaptureMouseFromApp(!ImGui::IsItemHovered());
	ImGui::End();
}

void NFGE::Editor::ShowUtil()
{
	ImGui::Begin("FPS");
	
	static auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
	lastTime = currentTime;

	ImGui::Text("FPS: %f", 1.0f / deltaTime);

	ImGui::End();
}

void NFGE::Editor::CameraControl(NFGE::Graphics::Camera & camera, float deltaTime)
{
	const float moveSpeed = 10.0f;
	const float turnSpeed = 5.0f;
	auto inputSystem = NFGE::Input::InputSystem::Get();
	if (inputSystem->IsKeyDown(NFGE::Input::KeyCode::W))
		camera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(NFGE::Input::KeyCode::S))
		camera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(NFGE::Input::KeyCode::D))
		camera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(NFGE::Input::KeyCode::A))
		camera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(NFGE::Input::MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		camera.Pitch(-inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void NFGE::Editor::ShowGameObjectInWorldView( GameObject* gameObject, std::string& moveTo, bool& isDraging, bool& isDroped, uint32_t depth)
{
	bool isDragingChild = false;

	ImGuiSelectableFlags selecctable_flags = 0;
	selecctable_flags |= ImGuiSelectableFlags_AllowDoubleClick;

	std::string displayName = "";
	for (size_t i = 0; i < depth; i++)
	{
		displayName += "\t";
	}
	displayName += gameObject->GetName();
	
	if (ImGui::Selectable(displayName.c_str(), gameObject == mSelectedGameObject, selecctable_flags))
	{
		mSelectedGameObject = gameObject;
		gameObject->mIsShowChildInEditor = !gameObject->mIsShowChildInEditor;
	}

	if (!gameObject->mChilds.empty() && gameObject->mIsShowChildInEditor)
	{
		for (auto child : gameObject->mChilds)
		{
			ShowGameObjectInWorldView(child, moveTo, isDragingChild, isDroped, depth + 1);
		}
	}

	

	if (ImGui::IsItemHovered())
	{
		//moveTo = gameObject->GetName();
		if (ImGui::IsMouseClicked(1))
		{
			isShowPop = true;
		}
	}
	else
	{
		if (ImGui::IsMouseClicked(0) && ImGui::IsMouseClicked(1))
		{
			isShowPop = false;
		}
	}

	if (isShowPop && mSelectedGameObject == gameObject)
	{
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Delete"))
			{
				mWorld.Destroy(mSelectedGameObject->GetHandle());
			}
			ImGui::EndPopup();
		}
	}

	if (!isDragingChild) // If is draging one of its child, don't drag itself. Otherwise it will overwrite the payLoad
	{
		ImGuiDragDropFlags src_flags = 0;
		src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
		src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
																  //src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip
		if (ImGui::BeginDragDropSource(src_flags))
		{
			isDraging = true;

			if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
				ImGui::Text("Moving \"%s\"", gameObject->GetName().c_str());
			std::string name = gameObject->GetName();
			ImGui::SetDragDropPayload("Choosing PayLoad", &name, sizeof(std::string));
			ImGui::EndDragDropSource();
		}
	}
	else
	{
		isDraging = isDragingChild; // keep passing the isDraging parameters
	}
	

	if (!isDroped)
	{
		if (ImGui::BeginDragDropTarget())
		{
			isDroped = true;
			ImGuiDragDropFlags target_flags = 0;
			//target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
			//target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
			//target_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Choosing PayLoad", target_flags))
			{
				auto movingFrom = *(std::string*)payload->Data;
				auto movingGameObj = mWorld.Find(movingFrom);
				
				auto moveToGameObj = mWorld.Find(gameObject->GetName());
				moveToGameObj->AddChild(movingGameObj.Get());
				
			}
			ImGui::EndDragDropTarget();
		}
	}
	
}
