/*==============================================================================

  Program: 3D Slicer

  Copyright (c) 2010 Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <QHeaderView>

// CTK includes
#include "ctkModelTester.h"

// qMRML includes
#include "qMRMLSceneModel.h"
#include "qMRMLSortFilterProxyModel.h"
#include "qMRMLSceneTransformModel.h"
#include "qMRMLSceneIGTLConnectorModel.h"
#include "qMRMLTreeView.h"

// OpenIGTLinkIF GUI includes
#include "qMRMLIGTLIOTreeView.h"

// OpenIGTLinkIF Logic includes
#include "vtkSlicerOpenIGTLinkIFLogic.h"

// MRML includes
#include <vtkMRMLNode.h>

//------------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_OpenIGTLinkIF
class qMRMLIGTLIOTreeViewPrivate
{
  Q_DECLARE_PUBLIC(qMRMLIGTLIOTreeView);
protected:
  qMRMLIGTLIOTreeView* const q_ptr;
public:
  qMRMLIGTLIOTreeViewPrivate(qMRMLIGTLIOTreeView& object);
  void init();

  qMRMLSceneIGTLConnectorModel*     SceneModel;
  qMRMLSortFilterProxyModel*        SortFilterModel;
  vtkSlicerOpenIGTLinkIFLogic*      Logic;
};

//------------------------------------------------------------------------------
qMRMLIGTLIOTreeViewPrivate::qMRMLIGTLIOTreeViewPrivate(qMRMLIGTLIOTreeView& object)
  : q_ptr(&object)
{
  this->SceneModel = 0;
  this->SortFilterModel = 0;
}

//------------------------------------------------------------------------------
void qMRMLIGTLIOTreeViewPrivate::init()
{
  Q_Q(qMRMLIGTLIOTreeView);

  this->SceneModel = new qMRMLSceneIGTLConnectorModel(q);
  q->setSceneModel(this->SceneModel, "IGTLConnector");
  //this->SortFilterModel = new qMRMLSortFilterProxyModel(q);
  // we only want to show vtkMRMLAnnotationNodes and vtkMRMLAnnotationHierarchyNodes
  QStringList nodeTypes = QStringList();
  nodeTypes.append("vtkMRMLIGTLConnectorNode");

  //this->SortFilterModel->setNodeTypes(nodeTypes);
  q->setNodeTypes(nodeTypes);
  this->SortFilterModel = q->sortFilterProxyModel();

  //this->SortFilterModel->setSourceModel(this->SceneModel);
  //q->qMRMLTreeView::setModel(this->SortFilterModel);

  //ctkModelTester * tester = new ctkModelTester(p);
  //tester->setModel(this->SortFilterModel);

//  QObject::connect(q, SIGNAL(clicked(const QModelIndex& )),
//                   q, SLOT(onClicked(const QModelIndex&)));

//  QObject::connect( q->selectionModel(),
//        SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection & ) ),
//        q,
//        SLOT( onSelectionChanged( const QItemSelection &, const QItemSelection & ) ),
//        Qt::DirectConnection );

  q->setUniformRowHeights(true);
}

//------------------------------------------------------------------------------
qMRMLIGTLIOTreeView::qMRMLIGTLIOTreeView(QWidget *_parent)
  : Superclass(_parent)
  , d_ptr(new qMRMLIGTLIOTreeViewPrivate(*this))
{
  Q_D(qMRMLIGTLIOTreeView);
  d->init();

  // we need to enable mouse tracking to set the appropriate cursor while mouseMove occurs
  this->setMouseTracking(true);
}

//------------------------------------------------------------------------------
qMRMLIGTLIOTreeView::~qMRMLIGTLIOTreeView()
{
}

//------------------------------------------------------------------------------
void qMRMLIGTLIOTreeView::setMRMLScene(vtkMRMLScene* scene)
{
  Q_D(qMRMLIGTLIOTreeView);
  Q_ASSERT(d->SortFilterModel);
  // only qMRMLSceneModel needs the scene, the other proxies don't care.
  d->SceneModel->setMRMLScene(scene);

  this->expandAll();
}

//------------------------------------------------------------------------------
//
// Click and selected event handling
//
//------------------------------------------------------------------------------


////------------------------------------------------------------------------------
//void qMRMLIGTLIOTreeView::onSelectionChanged(const QItemSelection& index,const QItemSelection& beforeIndex)
//{
//  Q_UNUSED(beforeIndex)

//  if (index.size() == 0)
//    {

//    // the user clicked in empty space of the treeView
//    // so we set the active hierarchy to the top level one
//    //this->m_Logic->SetActiveHierarchyNodeID(NULL);
//    }
//}

//------------------------------------------------------------------------------
//void qMRMLIGTLIOTreeView::onClicked(const QModelIndex& index)
//{

//  Q_D(qMRMLIGTLIOTreeView);

//  vtkMRMLNode *mrmlNode = d->SortFilterModel->mrmlNodeFromIndex(index);
//  if (!mrmlNode)
//    {
//    return;
//    }
//  // if the user clicked on a hierarchy, set this as the active one
//  // this means, new annotations or new user-created hierarchies will be created
//  // as childs of this one

//  if(mrmlNode->IsA("vtkMRMLAnnotationHierarchyNode"))
//    {
//      //this->m_Logic->SetActiveHierarchyNodeID(mrmlNode->GetID());
//    }
//  else
//    {
//    // if the user clicked on a row that isn't a hierarchy node, reset the
//    // active hierarchy to the parent hierarchy of this node (going via the
//    // hierarchy node associated with this node)
//    if(mrmlNode &&
//       !mrmlNode->IsA("vtkMRMLAnnotationHierarchyNode"))
//      {
//      //vtkMRMLHierarchyNode *hnode = vtkMRMLAnnotationHierarchyNode::GetAssociatedHierarchyNode(this->mrmlScene(), mrmlNode->GetID());
//      //if (hnode && hnode->GetParentNode())
//      //  {
//      //  this->m_Logic->SetActiveHierarchyNodeID(hnode->GetParentNode()->GetID());
//      //  }
//      }
//    }

//  if (index.column() == qMRMLSceneIGTLConnectorModel::NameColumn)
//    {
//    }
//  else if (index.column() == qMRMLSceneIGTLConnectorModel::TypeColumn)
//    {
//    //this->onVisibilityColumnClicked(mrmlNode);
//    }
//  else if (index.column() == qMRMLSceneIGTLConnectorModel::StatusColumn)
//    {
//    //this->onLockColumnClicked(mrmlNode);
//    }
//  else if (index.column() == qMRMLSceneIGTLConnectorModel::AddressColumn)
//    {
//    }
//  else if (index.column() == qMRMLSceneIGTLConnectorModel::PortColumn)
//    {
//    }

//}

////------------------------------------------------------------------------------
//const char* qMRMLIGTLIOTreeView::firstSelectedNode()
//{
//  Q_D(qMRMLIGTLIOTreeView);
//  QModelIndexList selected = this->selectedIndexes();

//  // first, check if we selected anything
//  if (selected.isEmpty())
//    {
//    return 0;
//    }

//  // now get the first selected item
//  QModelIndex index = selected.first();

//  // check if it is a valid node
//  if (!d->SortFilterModel->mrmlNodeFromIndex(index))
//    {
//    return 0;
//    }

//  return d->SortFilterModel->mrmlNodeFromIndex(index)->GetID();
//}

//------------------------------------------------------------------------------
//void qMRMLIGTLIOTreeView::toggleLockForSelected()
//{
//  Q_D(qMRMLIGTLIOTreeView);
//  QModelIndexList selected = this->selectedIndexes();

//  // first, check if we selected anything
//  if (selected.isEmpty())
//    {
//    return;
//    }

//  for (int i = 0; i < selected.size(); ++i) {

//    // we need to prevent looping through all columns
//    // there we only update once a row
//    //if (selected.at(i).column() == qMRMLSceneIGTLConnectorModel::LockColumn)
//    //  {
//    //
//    //  //vtkMRMLAnnotationNode* annotationNode = vtkMRMLAnnotationNode::SafeDownCast(d->SortFilterModel->mrmlNodeFromIndex(selected.at(i)));
//    //
//    //  //if (annotationNode)
//    //  //  {
//    //  //  this->onLockColumnClicked(annotationNode);
//    //  //  }
//    //
//    //  }

//  }

//}

////------------------------------------------------------------------------------
//void qMRMLIGTLIOTreeView::toggleVisibilityForSelected()
//{
//  Q_D(qMRMLIGTLIOTreeView);
//  QModelIndexList selected = this->selectedIndexes();

//  // first, check if we selected anything
//  if (selected.isEmpty())
//    {
//    return;
//    }

//  for (int i = 0; i < selected.size(); ++i)
//    {

//    // we need to prevent looping through all columns
//    // there we only update once a row
//    //if (selected.at(i).column() == qMRMLSceneIGTLConnectorModel::VisibilityColumn)
//    //  {
//    //
//    //  //vtkMRMLAnnotationNode* annotationNode = vtkMRMLAnnotationNode::SafeDownCast(d->SortFilterModel->mrmlNodeFromIndex(selected.at(i)));
//    //  //
//    //  //if (annotationNode)
//    //  //  {
//    //  //  this->onVisibilityColumnClicked(annotationNode);
//    //  //  }
//    //
//    //  }

//    } // for loop

//}

//------------------------------------------------------------------------------
//void qMRMLIGTLIOTreeView::deleteSelected()
//{
//  Q_D(qMRMLIGTLIOTreeView);
//  QModelIndexList selected = this->selectedIndexes();

//  QStringList markedForDeletion;

//  // first, check if we selected anything
//  if (selected.isEmpty())
//    {
//    return;
//    }

//  // case: delete a hierarchy only, if it is the only selection
//  // warning: all directly under this hierarchy laying annotation nodes will be lost
//  // if there are other hierarchies underneath the one which gets deleted, they will get reparented
//  if (selected.count()==6)
//    {
//    // only one item was selected, is this a hierarchy?
//    //vtkMRMLAnnotationHierarchyNode* hierarchyNode = vtkMRMLAnnotationHierarchyNode::SafeDownCast(d->SortFilterModel->mrmlNodeFromIndex(selected.first()));

//    //if (hierarchyNode)
//    //  {
//    //  // this is exciting!!
//    //
//    //  // get confirmation to delete
//    //  QMessageBox msgBox;
//    //  msgBox.setText("Do you really want to delete the selected hierarchy?");
//    //  msgBox.setInformativeText("This includes all directly associated annotations.");
//    //  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//    //  msgBox.setDefaultButton(QMessageBox::No);
//    //  int ret = msgBox.exec();
//    //
//    //  if (ret == QMessageBox::Yes)
//    //    {
//    //
//    //    hierarchyNode->DeleteDirectChildren();
//    //
//    //    this->mrmlScene()->RemoveNode(hierarchyNode);
//    //
//    //    }
//    //  // all done, bail out
//    //  return;
//    //  }
//    // if this is not a hierarchyNode, treat this single selection as a normal case

//    }
//  // end hierarchy case


//  // get confirmation to delete
//  QMessageBox msgBox;
//  msgBox.setText("Do you really want to delete the selected annotations?");
//  msgBox.setInformativeText("This does not include hierarchies.");
//  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//  msgBox.setDefaultButton(QMessageBox::No);
//  int ret = msgBox.exec();

//  if (ret == QMessageBox::No)
//    {
//    //bail out
//    return;
//    }

//  // case:: delete all selected annotationNodes but no hierarchies
//  for (int i = 0; i < selected.count(); ++i)
//    {

//    // we need to prevent looping through all columns
//    // there we only update once a row
//    //if (selected.at(i).column() == qMRMLSceneIGTLConnectorModel::VisibilityColumn)
//    //  {
//    //
//    //  //vtkMRMLAnnotationNode* annotationNode = vtkMRMLAnnotationNode::SafeDownCast(d->SortFilterModel->mrmlNodeFromIndex(selected.at(i)));
//    //
//    //  //if (annotationNode)
//    //  //  {
//    //  //
//    //  //  // we mark this one for deletion
//    //  //  markedForDeletion.append(QString(annotationNode->GetID()));
//    //  //
//    //  //  }
//    //
//    //  }
//    } // for

//  // we parsed the complete selection and saved all mrmlIds to delete
//  // now, it is safe to delete
//  for (int j=0; j < markedForDeletion.size(); ++j)
//    {

//    //vtkMRMLAnnotationNode* annotationNodeToDelete = vtkMRMLAnnotationNode::SafeDownCast(this->mrmlScene()->GetNodeByID(markedForDeletion.at(j).toLatin1()));
//    //this->m_Logic->RemoveAnnotationNode(annotationNodeToDelete);

//    }

//  //this->m_Logic->SetActiveHierarchyNodeID(NULL);

//}

//------------------------------------------------------------------------------
// Return the selected annotations as a collection of mrmlNodes
//------------------------------------------------------------------------------
//void qMRMLIGTLIOTreeView::selectedAsCollection(vtkCollection* collection)
//{

//  if (!collection)
//    {
//    return;
//    }

//  Q_D(qMRMLIGTLIOTreeView);
//  QModelIndexList selected = this->selectedIndexes();

//  // first, check if we selected anything
//  if (selected.isEmpty())
//    {
//    return;
//    }

//  qSort(selected.begin(),selected.end());

//  for (int i = 0; i < selected.size(); ++i)
//    {

//      // we need to prevent looping through all columns
//      // there we only update once a row
//      //if (selected.at(i).column() == qMRMLSceneIGTLConnectorModel::VisibilityColumn)
//      //  {
//      //
//      //  vtkMRMLNode* node = vtkMRMLNode::SafeDownCast(d->SortFilterModel->mrmlNodeFromIndex(selected.at(i)));
//      //
//      //  //if (node->IsA("vtkMRMLAnnotationNode"))
//      //   // {
//      //  collection->AddItem(node);
//      //   // }
//      //
//      //  }

//    } // for

//}

//------------------------------------------------------------------------------
//
// MouseMove event handling
//
//------------------------------------------------------------------------------

#ifndef QT_NO_CURSOR
//------------------------------------------------------------------------------
bool qMRMLIGTLIOTreeView::viewportEvent(QEvent* e)
{

  // reset the cursor if we leave the viewport
  if(e->type() == QEvent::Leave)
    {
    setCursor(QCursor());
    }

  return QTreeView::viewportEvent(e);
}

//------------------------------------------------------------------------------
void qMRMLIGTLIOTreeView::mouseMoveEvent(QMouseEvent* e)
{
  this->QTreeView::mouseMoveEvent(e);

  // get the index of the current column
  QModelIndex index = indexAt(e->pos());

  //if (index.column() == qMRMLSceneIGTLConnectorModel::VisibilityColumn || index.column() == qMRMLSceneIGTLConnectorModel::LockColumn || index.column() == qMRMLSceneIGTLConnectorModel::EditColumn)
  //  {
  //  // we are over a column with a clickable icon
  //  // let's change the cursor
  //  QCursor handCursor(Qt::PointingHandCursor);
  //  this->setCursor(handCursor);
  //  // .. and bail out
  //  return;
  //  }
  //else if(this->cursor().shape() == Qt::PointingHandCursor)
  //  {
  //  // if we are NOT over such a column and we already have a changed cursor,
  //  // reset it!
  //  this->setCursor(QCursor());
  //  }

}
#endif

//------------------------------------------------------------------------------
//
// Layout and behavior customization
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void qMRMLIGTLIOTreeView::setSelectedNode(const char* id)
{
  Q_D(qMRMLIGTLIOTreeView);

  vtkMRMLNode* node = this->mrmlScene()->GetNodeByID(id);

  if (node)
    {
    this->setCurrentIndex(d->SortFilterModel->indexFromMRMLNode(node));
    }
}

//------------------------------------------------------------------------------
//
// In-Place Editing of Annotations
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//void qMRMLIGTLIOTreeView::onVisibilityColumnClicked(vtkMRMLNode* node)
//{
//  if (!node)
//    {
//    // no node found!
//    return;
//    }

//  //vtkMRMLAnnotationNode* annotationNode = vtkMRMLAnnotationNode::SafeDownCast(node);

//  //if (annotationNode)
//  //  {
//  //  // this is a valid annotationNode
//  //  annotationNode->SetVisible(!annotationNode->GetVisible());
//  //
//  //  }


//  // TODO move to logic
//  //vtkMRMLAnnotationHierarchyNode* hierarchyNode = vtkMRMLAnnotationHierarchyNode::SafeDownCast(node);
//  //
//  //if (hierarchyNode)
//  //  {
//  //  vtkCollection* children = vtkCollection::New();
//  //  hierarchyNode->GetChildrenDisplayableNodes(children);
//  //
//  //  children->InitTraversal();
//  //  for (int i=0; i<children->GetNumberOfItems(); ++i)
//  //    {
//  //    vtkMRMLAnnotationNode* childNode = vtkMRMLAnnotationNode::SafeDownCast(children->GetItemAsObject(i));
//  //    if (childNode)
//  //      {
//  //      // this is a valid annotation child node
//  //      //
//  //      childNode->SetVisible(!childNode->GetVisible());
//  //      }
//  //    } // for loop
//  //
//  //  } // if hierarchyNode
//}

//------------------------------------------------------------------------------
//void qMRMLIGTLIOTreeView::onLockColumnClicked(vtkMRMLNode* node)
//{

//  if (!node)
//    {
//    // no node found!
//    return;
//    }

//  //vtkMRMLAnnotationNode* annotationNode = vtkMRMLAnnotationNode::SafeDownCast(node);
//  //
//  //if (annotationNode)
//  //  {
//  //  // this is a valid annotationNode
//  //  annotationNode->SetLocked(!annotationNode->GetLocked());
//  //
//  //  }


//  // TODO move to logic
//  //vtkMRMLAnnotationHierarchyNode* hierarchyNode = vtkMRMLAnnotationHierarchyNode::SafeDownCast(node);
//  //
//  //if (hierarchyNode)
//  //  {
//  //  vtkCollection* children = vtkCollection::New();
//  //  hierarchyNode->GetChildrenDisplayableNodes(children);
//  //
//  //  children->InitTraversal();
//  //  for (int i=0; i<children->GetNumberOfItems(); ++i)
//  //    {
//  //    vtkMRMLAnnotationNode* childNode = vtkMRMLAnnotationNode::SafeDownCast(children->GetItemAsObject(i));
//  //    if (childNode)
//  //      {
//  //      // this is a valid annotation child node
//  //      //
//  //      childNode->SetLocked(!childNode->GetLocked());
//  //      }
//  //    } // for loop
//  //
//  //  } // if hierarchyNode
//}

//------------------------------------------------------------------------------
void qMRMLIGTLIOTreeView::mousePressEvent(QMouseEvent* event)
{
  // skip qMRMLTreeView
  this->QTreeView::mousePressEvent(event);
}

//------------------------------------------------------------------------------
//
// Connections to other classes
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Set and observe the logic
//-----------------------------------------------------------------------------
void qMRMLIGTLIOTreeView::setLogic(vtkSlicerOpenIGTLinkIFLogic* logic)
{
  Q_D(qMRMLIGTLIOTreeView);
  if (!logic)
    {
    return;
    }

  d->Logic = logic;

  // propagate down to model
  d->SceneModel->setLogic(d->Logic);
}