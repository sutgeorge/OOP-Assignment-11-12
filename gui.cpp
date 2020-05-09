#include "gui.h"
#include <iostream>
#include <vector>
using namespace std;

GUI::GUI(Service* _service, QWidget *parent) :
    QWidget(parent),
  service(_service)
{
    this->initGUI();
    this->connect_signals_and_slots();
}


void GUI::initGUI() {
    QHBoxLayout* main_layout = new QHBoxLayout(this);
    QVBoxLayout* element_addition_layout = new QVBoxLayout();
    QFormLayout* element_addition_form = new QFormLayout();
    QGridLayout* buttons_layout = new QGridLayout();

    QString add_text("Add");
    this->add_button = new QPushButton(add_text);
    buttons_layout->addWidget(add_button, 1, 1);

    QString update_text("Update");
    this->update_button = new QPushButton(update_text);
    buttons_layout->addWidget(update_button, 1, 2);

    QString delete_text("Delete");
    this->delete_button = new QPushButton(delete_text);
    buttons_layout->addWidget(delete_button, 1, 3);




    QLabel* elements = new QLabel("Recordings:");
    element_addition_layout->addWidget(elements);

    this->recordings_list = new QListWidget;
    element_addition_layout->addWidget(this->recordings_list);

    QLabel* title_label = new QLabel("Title: ");
    this->title_edit = new QLineEdit();
    element_addition_form->addRow(title_label, this->title_edit);

    QLabel* location_label = new QLabel("Location: ");
    this->location_edit = new QLineEdit();
    element_addition_form->addRow(location_label, this->location_edit);

    QLabel* time_of_creation_label = new QLabel("Time of creation: ");
    this->time_of_creation_edit = new QLineEdit();
    element_addition_form->addRow(time_of_creation_label, this->time_of_creation_edit);

    QLabel* times_accessed_label = new QLabel("Times accessed: ");
    this->times_accessed_edit = new QLineEdit();
    element_addition_form->addRow(times_accessed_label, this->times_accessed_edit);

    QLabel* footage_preview_label = new QLabel("Footage preview: ");
    this->footage_preview_edit = new QLineEdit();
    element_addition_form->addRow(footage_preview_label, this->footage_preview_edit);


    element_addition_layout->addLayout(element_addition_form);
    element_addition_layout->addLayout(buttons_layout);
    main_layout->addLayout(element_addition_layout);
}


void GUI::connect_signals_and_slots() {
    /// Connects the add button to its corresponding handler
    QObject::connect(this->add_button, &QPushButton::clicked, this, &GUI::add_recording_button_handler);

    /// Changes the text in the line edits whenever a list widget item is selected
    QObject::connect(this->recordings_list, &QListWidget::itemSelectionChanged, this, &GUI::update_line_edits);
}


void GUI::add_recording_button_handler() {
    string title = this->title_edit->text().toStdString();
    string location = this->location_edit->text().toStdString();
    string time_of_creation = this->time_of_creation_edit->text().toStdString();
    string times_accessed = this->times_accessed_edit->text().toStdString();
    string footage_preview = this->footage_preview_edit->text().toStdString();

    try {
        string message =  title + " " + location + " " + time_of_creation + " " + times_accessed + " " +footage_preview;
        qDebug() << QString::fromStdString(message);
        service->add(title, location, time_of_creation, times_accessed, footage_preview);
    } catch (...) {

    }

    this->add_recordings_to_list_widget();
}


void GUI::add_recordings_to_list_widget() {
    if (this->recordings_list->count() > 0) {
        this->recordings_list->clear();
    }

    vector<Recording> container = service->get_repository_container();

    for (auto recording: container) {
        string recording_as_string = recording.get_title() + "; " + recording.get_location() + "; " + recording.get_time_of_creation() + "; " + std::to_string(recording.get_times_accessed()) + "; " + recording.get_footage_preview();
        QString item_in_list = QString::fromStdString(recording_as_string);
        QListWidgetItem* recording_list_item = new QListWidgetItem(item_in_list);
        this->recordings_list->addItem(recording_list_item);
    }

    if (this->recordings_list->count() > 0) {
        this->recordings_list->setCurrentRow(0);
    }
}


int GUI::get_current_index()
{
    if (this->recordings_list->count() == 0)
        return -1;

    // get selected index
    QModelIndexList index = this->recordings_list->selectionModel()->selectedIndexes();
    if (index.size() == 0)
    {
        this->title_edit->clear();
        this->location_edit->clear();
        this->time_of_creation_edit->clear();
        this->times_accessed_edit->clear();
        this->footage_preview_edit->clear();
        return -1;
    }

    int idx = index.at(0).row();
    return idx;
}


void GUI::update_line_edits() {
    vector<Recording> container = service->get_repository_container();
    int item_index = this->get_current_index();

    if (item_index == -1) {
        return;
    }

    if (item_index >= container.size()) {
        return;
    }

    Recording recording = container[item_index];
    this->title_edit->setText(QString::fromStdString(recording.get_title()));
    this->location_edit->setText(QString::fromStdString(recording.get_location()));
    this->time_of_creation_edit->setText(QString::fromStdString(recording.get_time_of_creation()));
    this->times_accessed_edit->setText(QString::fromStdString(std::to_string(recording.get_times_accessed())));
    this->footage_preview_edit->setText(QString::fromStdString(recording.get_footage_preview()));
}


GUI::~GUI()
{
}
