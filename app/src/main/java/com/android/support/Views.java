package com.android.support;

import static android.view.ViewGroup.LayoutParams.MATCH_PARENT;
import static android.view.ViewGroup.LayoutParams.WRAP_CONTENT;

import android.app.AlertDialog;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.Typeface;
import android.os.Build;
import android.text.Html;
import android.text.InputFilter;
import android.text.InputType;
import android.text.TextUtils;
import android.text.method.DigitsKeyListener;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Objects;

public class Views extends Menu {


    public static View Switch(int featNum, String featName, boolean swiOn) {
        final Switch switchR = new Switch(Preferences.context);
        ColorStateList buttonStates = new ColorStateList(
                new int[][]{
                        new int[]{-android.R.attr.state_enabled},
                        new int[]{android.R.attr.state_checked},
                        new int[]{}
                },
                new int[]{
                        Color.BLUE,
                        ToggleON, // ON
                        ToggleOFF // OFF
                }
        );
        //Set colors of the switch. Comment out if you don't like it
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            switchR.getThumbDrawable().setTintList(buttonStates);
            switchR.getTrackDrawable().setTintList(buttonStates);
        }
        switchR.setText(featName);
        switchR.setTextColor(Color.parseColor("#FFFFFF"));
        switchR.setPadding(10, 5, 0, 5);
        switchR.setChecked(Preferences.loadPrefBool(featName, featNum, swiOn));
        switchR.setOnCheckedChangeListener((compoundButton, bool) -> {
            Preferences.changeFeatureBool(featName, featNum, bool);
            switch (featNum) {
                case -1: //Save perferences
                    Preferences.with(switchR.getContext()).writeBoolean(-1, bool);
                    if (!bool)
                        Preferences.with(switchR.getContext()).clear(); //Clear perferences if switched off
                    break;
                case -3:
                    Preferences.isExpanded = bool;
                    scrollView.setLayoutParams(Preferences.isExpanded ? scrlLLExpanded : scrlLL);
                    break;
            }
        });
        return switchR;
    }

    public static View SeekBar(final int featNum, final String featName, final int min, int max) {
        int loadedProg = Preferences.loadPrefInt(featName, featNum);
        LinearLayout linearLayout = new LinearLayout(Preferences.context);
        linearLayout.setPadding(10, 5, 0, 5);
        linearLayout.setOrientation(LinearLayout.VERTICAL);
        linearLayout.setGravity(Gravity.CENTER);

        final TextView textView = new TextView(Preferences.context);
        textView.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + ((loadedProg == 0) ? min : loadedProg)));
        textView.setTextColor(Color.parseColor("#FFFFFF"));

        SeekBar seekBar = new SeekBar(Preferences.context);
        seekBar.setPadding(25, 10, 35, 10);
        seekBar.setMax(max);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O)
            seekBar.setMin(min); //setMin for Oreo and above
        seekBar.setProgress((loadedProg == 0) ? min : loadedProg);
        seekBar.getThumb().setColorFilter(SeekBarColor, PorterDuff.Mode.SRC_ATOP);
        seekBar.getProgressDrawable().setColorFilter(SeekBarProgressColor, PorterDuff.Mode.SRC_ATOP);
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
            }

            public void onProgressChanged(SeekBar seekBar, int i, boolean z) {
                //if progress is greater than minimum, don't go below. Else, set progress
                seekBar.setProgress(Math.max(i, min));
                Preferences.changeFeatureInt(featName, featNum, Math.max(i, min));
                textView.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + (Math.max(i, min))));
            }
        });
        linearLayout.addView(textView);
        linearLayout.addView(seekBar);

        return linearLayout;
    }

    public static View Button(final int featNum, final String featName) {
        final Button button = new Button(Preferences.context);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(MATCH_PARENT, MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);
        button.setLayoutParams(layoutParams);
        button.setTextColor(Color.parseColor("#FFFFFF"));
        button.setAllCaps(false); //Disable caps to support html
        button.setText(Html.fromHtml(featName));
        button.setBackgroundColor(BTN_COLOR);
        button.setOnClickListener(v -> {
            switch (featNum) {
                case -4:
                    Logcat.Save(Preferences.context);
                    break;
                case -5:
                    Logcat.Clear(Preferences.context);
                    break;
                case -6:
                    scrollView.removeView(mSettings);
                    scrollView.addView(patches);
                    break;
                case -100:
                    stopChecking = true;
                    break;
            }
            Preferences.changeFeatureInt(featName, featNum, 0);
        });

        return button;
    }

    public static View ButtonOnOff(final int featNum, String featName, boolean switchedOn) {
        final Button button = new Button(Preferences.context);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(MATCH_PARENT, MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);
        button.setLayoutParams(layoutParams);
        button.setTextColor(Color.parseColor("#FFFFFF"));
        button.setAllCaps(false); //Disable caps to support html

        final String finalfeatName = featName.replace("OnOff_", "");
        boolean isOn = Preferences.loadPrefBool(featName, featNum, switchedOn);
        if (isOn) {
            button.setText(Html.fromHtml(finalfeatName + ": ON"));
            button.setBackgroundColor(BtnON);
            isOn = false;
        } else {
            button.setText(Html.fromHtml(finalfeatName + ": OFF"));
            button.setBackgroundColor(BtnOFF);
            isOn = true;
        }
        final boolean finalIsOn = isOn;
        button.setOnClickListener(new View.OnClickListener() {
            boolean isOn = finalIsOn;

            public void onClick(View v) {
                Preferences.changeFeatureBool(finalfeatName, featNum, isOn);
                //Log.d(TAG, finalfeatName + " " + featNum + " " + isActive2);
                if (isOn) {
                    button.setText(Html.fromHtml(finalfeatName + ": ON"));
                    button.setBackgroundColor(BtnON);
                    isOn = false;
                } else {
                    button.setText(Html.fromHtml(finalfeatName + ": OFF"));
                    button.setBackgroundColor(BtnOFF);
                    isOn = true;
                }
            }
        });
        return button;
    }

    public static View Spinner(final int featNum, final String featName, final String list) {
        Log.d(TAG, "spinner " + featNum + " " + featName + " " + list);
        final List<String> lists = new LinkedList<>(Arrays.asList(list.split(",")));

        // Create another LinearLayout as a workaround to use it as a background
        // to keep the down arrow symbol. No arrow symbol if setBackgroundColor set
        LinearLayout linearLayout2 = new LinearLayout(Preferences.context);
        LinearLayout.LayoutParams layoutParams2 = new LinearLayout.LayoutParams(MATCH_PARENT, WRAP_CONTENT);
        layoutParams2.setMargins(7, 2, 7, 5);
        linearLayout2.setOrientation(LinearLayout.VERTICAL);
        linearLayout2.setBackgroundColor(BTN_COLOR);
        linearLayout2.setLayoutParams(layoutParams2);

        final Spinner spinner = new Spinner(Preferences.context, Spinner.MODE_DROPDOWN);
        spinner.setLayoutParams(layoutParams2);
        spinner.getBackground().setColorFilter(1, PorterDuff.Mode.SRC_ATOP); //trick to show white down arrow color
        //Creating the ArrayAdapter instance having the list
        ArrayAdapter<String> aa = new ArrayAdapter<>(Preferences.context, android.R.layout.simple_spinner_dropdown_item, lists);
        aa.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        //Setting the ArrayAdapter data on the Spinner'
        spinner.setAdapter(aa);
        spinner.setSelection(Preferences.loadPrefInt(featName, featNum));
        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parentView, View selectedItemView, int position, long id) {
                Preferences.changeFeatureInt(spinner.getSelectedItem().toString(), featNum, position);
                ((TextView) parentView.getChildAt(0)).setTextColor(Color.parseColor("#FFFFFF"));
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
            }
        });
        linearLayout2.addView(spinner);
        return linearLayout2;
    }

    public static View TextField(final int featNum, final String featName, final boolean numOnly, final int maxValue) {
        final Menu.EditTextString edittextstring = new Menu.EditTextString();
        final Menu.EditTextNum edittextnum = new Menu.EditTextNum();
        LinearLayout linearLayout = new LinearLayout(Preferences.context);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(MATCH_PARENT, MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);

        final Button button = new Button(Preferences.context);
        if (numOnly) {
            int num = Preferences.loadPrefInt(featName, featNum);
            edittextnum.setNum((num == 0) ? 1 : num);
            button.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + ((num == 0) ? 1 : num) + "</font>"));
        } else {
            String string = Preferences.loadPrefString(featName, featNum);
            edittextstring.setString(string);
            button.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + string + "</font>"));
        }
        button.setAllCaps(false);
        button.setLayoutParams(layoutParams);
        button.setBackgroundColor(BTN_COLOR);
        button.setTextColor(Color.parseColor("#FFFFFF"));
        button.setOnClickListener(view -> {
            final AlertDialog alert = new AlertDialog.Builder(Preferences.context, 2).create();
            Objects.requireNonNull(alert.getWindow()).setType(Build.VERSION.SDK_INT >= 26 ? 2038 : 2002);
            alert.setOnCancelListener(dialog -> {
                InputMethodManager imm = (InputMethodManager) Preferences.context.getSystemService(INPUT_METHOD_SERVICE);
                imm.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0);
            });

            //LinearLayout
            LinearLayout linearLayout1 = new LinearLayout(Preferences.context);
            linearLayout1.setPadding(5, 5, 5, 5);
            linearLayout1.setOrientation(LinearLayout.VERTICAL);
            linearLayout1.setBackgroundColor(MENU_FEATURE_BG_COLOR);

            //TextView
            final TextView TextViewNote = new TextView(Preferences.context);
            TextViewNote.setText("Tap OK to apply changes. Tap outside to cancel");
            if (maxValue != 0)
                TextViewNote.setText("Tap OK to apply changes. Tap outside to cancel\nMax value: " + maxValue);
            TextViewNote.setTextColor(Color.parseColor("#FFFFFF"));

            //Edit text
            final EditText edittext = new EditText(Preferences.context);
            edittext.setMaxLines(1);
            edittext.setWidth(convertDipToPixels(300));
            edittext.setTextColor(Color.parseColor("#FFFFFF"));
            if (numOnly) {
                edittext.setInputType(InputType.TYPE_CLASS_NUMBER);
                edittext.setKeyListener(DigitsKeyListener.getInstance("0123456789-"));
                InputFilter[] FilterArray = new InputFilter[1];
                FilterArray[0] = new InputFilter.LengthFilter(10);
                edittext.setFilters(FilterArray);
            } else {
                edittext.setText(edittextstring.getString());
            }
            edittext.setOnFocusChangeListener((v, hasFocus) -> {
                InputMethodManager imm = (InputMethodManager) Preferences.context.getSystemService(INPUT_METHOD_SERVICE);
                if (hasFocus) {
                    imm.toggleSoftInput(InputMethodManager.SHOW_FORCED, InputMethodManager.HIDE_IMPLICIT_ONLY);
                } else {
                    imm.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0);
                }
            });
            edittext.requestFocus();

            //Button
            Button btndialog = new Button(Preferences.context);
            btndialog.setBackgroundColor(BTN_COLOR);
            btndialog.setTextColor(Color.parseColor("#FFFFFF"));
            btndialog.setText("OK");
            btndialog.setOnClickListener(view1 -> {
                if (numOnly) {
                    int num;
                    try {
                        num = Integer.parseInt(TextUtils.isEmpty(edittext.getText().toString()) ? "0" : edittext.getText().toString());
                        if (maxValue != 0 && num >= maxValue)
                            num = maxValue;
                    } catch (NumberFormatException ex) {
                        num = 2147483640;
                    }
                    edittextnum.setNum(num);
                    button.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + num + "</font>"));
                    alert.dismiss();
                    Preferences.changeFeatureInt(featName, featNum, num);
                } else {
                    String str = edittext.getText().toString();
                    edittextstring.setString(edittext.getText().toString());
                    button.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + str + "</font>"));
                    alert.dismiss();
                    Preferences.changeFeatureString(featName, featNum, str);
                }
                edittext.setFocusable(false);
            });

            linearLayout1.addView(TextViewNote);
            linearLayout1.addView(edittext);
            linearLayout1.addView(btndialog);
            alert.setView(linearLayout1);
            alert.show();
        });

        linearLayout.addView(button);
        return linearLayout;
    }

    public static View CheckBox(final int featNum, final String featName, boolean switchedOn) {
        final CheckBox checkBox = new CheckBox(Preferences.context);
        checkBox.setText(featName);
        checkBox.setTextColor(Color.parseColor("#FFFFFF"));
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP)
            checkBox.setButtonTintList(ColorStateList.valueOf(CheckBoxColor));
        checkBox.setChecked(Preferences.loadPrefBool(featName, featNum, switchedOn));
        checkBox.setOnCheckedChangeListener((buttonView, isChecked) -> {
            if (checkBox.isChecked()) {
                Preferences.changeFeatureBool(featName, featNum, isChecked);
            } else {
                Preferences.changeFeatureBool(featName, featNum, isChecked);
            }
        });
        return checkBox;
    }

    public static View RadioButton(final int featNum, String featName, final String list) {
        final List<String> lists = new LinkedList<>(Arrays.asList(list.split(",")));

        final TextView textView = new TextView(Preferences.context);
        textView.setText(featName + ":");
        textView.setTextColor(Color.parseColor("#FFFFFF"));

        final RadioGroup radioGroup = new RadioGroup(Preferences.context);
        radioGroup.setPadding(10, 5, 10, 5);
        radioGroup.setOrientation(LinearLayout.VERTICAL);
        radioGroup.addView(textView);

        for (int i = 0; i < lists.size(); i++) {
            final RadioButton Radioo = new RadioButton(Preferences.context);
            final String finalfeatName = featName, radioName = lists.get(i);
            View.OnClickListener first_radio_listener = v -> {
                textView.setText(Html.fromHtml(finalfeatName + ": <font color='" + NumberTxtColor + "'>" + radioName));
                Preferences.changeFeatureInt(finalfeatName, featNum, radioGroup.indexOfChild(Radioo));
            };
            System.out.println(lists.get(i));
            Radioo.setText(lists.get(i));
            Radioo.setTextColor(Color.LTGRAY);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP)
                Radioo.setButtonTintList(ColorStateList.valueOf(RadioColor));
            Radioo.setOnClickListener(first_radio_listener);
            radioGroup.addView(Radioo);
        }

        int index = Preferences.loadPrefInt(featName, featNum);
        if (index > 0) { //Preventing it to get an index less than 1. below 1 = null = crash
            textView.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + lists.get(index - 1)));
            ((RadioButton) radioGroup.getChildAt(index)).setChecked(true);
        }

        return radioGroup;
    }

    public static void Collapse(LinearLayout linLayout, final String text) {
        LinearLayout.LayoutParams layoutParamsLL = new LinearLayout.LayoutParams(MATCH_PARENT, MATCH_PARENT);
        layoutParamsLL.setMargins(0, 5, 0, 0);

        LinearLayout collapse = new LinearLayout(Preferences.context);
        collapse.setLayoutParams(layoutParamsLL);
        collapse.setVerticalGravity(16);
        collapse.setOrientation(LinearLayout.VERTICAL);

        final LinearLayout collapseSub = new LinearLayout(Preferences.context);
        collapseSub.setVerticalGravity(16);
        collapseSub.setPadding(0, 5, 0, 5);
        collapseSub.setOrientation(LinearLayout.VERTICAL);
        collapseSub.setBackgroundColor(Color.parseColor("#222D38"));
        collapseSub.setVisibility(View.GONE);
        mCollapse = collapseSub;

        final TextView textView = new TextView(Preferences.context);
        textView.setBackgroundColor(MENU_FEATURE_BG_COLOR);
        textView.setText("▽ " + text + " ▽");
        textView.setGravity(Gravity.CENTER);
        textView.setTextColor(ToggleON);
        textView.setTypeface(null, Typeface.BOLD);
        textView.setPadding(0, 20, 0, 20);
        textView.setOnClickListener(new View.OnClickListener() {
            boolean isChecked;

            @Override
            public void onClick(View v) {

                boolean z = !this.isChecked;
                this.isChecked = z;
                if (z) {
                    collapseSub.setVisibility(View.VISIBLE);
                    textView.setText("△ " + text + " △");
                    return;
                }
                collapseSub.setVisibility(View.GONE);
                textView.setText("▽ " + text + " ▽");
            }
        });
        collapse.addView(textView);
        collapse.addView(collapseSub);
        linLayout.addView(collapse);
    }
}

